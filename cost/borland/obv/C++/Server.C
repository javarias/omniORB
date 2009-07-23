// Server.C

#include "map_s.hh"
#include <math.h>
#include "PntImpl.h"

// convenience operator
CORBA::Boolean operator ==(const Map::Label& label_1, 
                           const Map::Label& label_2)
{
   return !strcmp(label_1._value(), label_2._value());   
}


// PointManager implementation
class PointManagerImpl : public POA_Map::PointManager
{
   typedef enum eExceptType {
      INVALID_POINT,
      UNKNOWN_POINT,
      DUPLICATE_POINT
    } eExceptType;

  public:
     PointManagerImpl(const char * manager_name, CORBA::Boolean verbose) 
        : _manager_name(CORBA::string_dup(manager_name)), _verbose(verbose)
     {}
  
     ~PointManagerImpl() {}
  
     // create a point
     Map::Point_ptr create_point(CORBA::Long x, 
                                 CORBA::Long y,
           Map::Label_ptr lab , 
                                 CORBA::Boolean connectable) {
        if (!lab)
           raiseException(INVALID_POINT);
        
        if (_verbose) {
          cout << "PointManager::creating [" 
               << connectable ? "connected" : "disconnected" ;
          cout <<  "] point for [";
          cout << lab << ", " << (long)x << ", " << (long)y << "]" << endl;
        }
  
        Map::Point_ptr point = _find_point(lab);
        if (point)
          raiseException(DUPLICATE_POINT);
        
        Map::PointSeq empty_points;
        empty_points.length(0);
        if (connectable) 
          point = new ConnectedPointImpl(x, y, lab, empty_points);
        else 
          point = new PointImpl(x, y, lab);

        _add_point(point);
  
        if (_verbose) 
          cout << "PointManager::created [" << point << "]" << endl;
  
        CORBA::add_ref(point);
        return point;
     }
  
     // Get
     Map::Point_ptr get_point (Map::Label_ptr lab) {
       if (!lab)
         raiseException(INVALID_POINT);

       Map::Point_ptr point = _find_point(lab);
       if (!point)
         raiseException(UNKNOWN_POINT, lab);
  
       CORBA::add_ref(point);
       return point;
    }
  
     // Destroy 
     void destroy_point (Map::Label_ptr lab){
      if (lab == NULL)
         raiseException(INVALID_POINT);
       
      if (_verbose)
        cout << "PointManager::destroy_point for [" << lab << "]" << endl;
  
      if (!_remove_point(lab))
        raiseException(UNKNOWN_POINT, lab);
     }
  
    // Returns a sequence of points given a scaling factor
    Map::PointSeq* list_points (CORBA::ULong scaling_factor) {
      if (_verbose)
        cout << "PointManager::list_points with scaling [" 
             << scaling_factor << "]";
  
      Map::PointSeq_var point_list = new Map::PointSeq;
      point_list->length(_points.length() * scaling_factor);
  
      for (CORBA::ULong i = 0; i < _points.length(); i++) {
        for (CORBA::ULong j = 0; j < scaling_factor; j++) {
          CORBA::add_ref(_points[i]);
          point_list[i * scaling_factor + j] = _points[i];
        }
      }
  
      return Map::PointSeq::_duplicate(point_list);
    }
  
    void connect_point(Map::Label_ptr lab, const Map::LabelSeq& connectTo) {
      if (!lab) 
        raiseException(INVALID_POINT);
  
      Map::Point_ptr point = _find_point(lab);
      if (!point)
        raiseException(UNKNOWN_POINT, lab);
      
      Map::ConnectedPoint_ptr conn_pt = Map::ConnectedPoint::_downcast(point);
      if (!conn_pt)
        raiseException(INVALID_POINT);
    
      CORBA::ULong length = connectTo.length();

      for (CORBA::ULong i = 0; i < length; i++) {
        Map::Point_ptr target = _find_point(connectTo[i]);
        if (!target)
          raiseException(UNKNOWN_POINT, connectTo[i]);
        conn_pt->add_connection(target);
      }
    }
  
    char* get_name() { return CORBA::string_dup(_manager_name); }
  
    CORBA::Long _find(Map::Label_ptr label) {
      CORBA::Long i =0;
      for (i = 0; i < _points.length(); i++) {
        if (strcmp(label->_value(), _points[i]->label()) == 0) 
          break;
      }
      return i < _points.length() ? i : -1;
    }
  
    CORBA::Boolean _remove_point(Map::Label_ptr label) {
      CORBA::Long index = _find(label);
      if (index >= _points.length() ||  index == -1)
        return (CORBA::Boolean)0;

      if (index < _points.length()-1)
        _points[index] = _points[_points.length()-1];
      else
        CORBA::remove_ref(_points[index]);
  
      _points.length(_points.length()-1);
      return 1UL;
    }
  
    Map::Point_ptr _find_point(Map::Label_ptr label) {
      CORBA::Long index = _find(label);
      return (index < _points.length() && index != -1) ? (Map::Point_ptr)_points[index] :
                                                         (Map::Point_ptr)NULL;
    }
  
    void _add_point(Map::Point_ptr p) {
      CORBA::ULong current = _points.length();
      _points.length(current+1);
      _points[current] = p;
    }
  
    void raiseException(eExceptType etype, 
                        Map::Label_ptr label = (Map::Label_ptr)NULL) {
      switch (etype) {
        case INVALID_POINT :
            throw Map::InvalidPoint();
        case UNKNOWN_POINT:
          if (label) {
            CORBA::add_ref( label );
            throw Map::UnknownPoint(label);
          }
          else
            throw Map::UnknownPoint();
        case DUPLICATE_POINT:
          throw Map::DuplicatePoint();
      }
    }

  private:
    Map::PointSeq      _points;
    CORBA::String_var  _manager_name;
    CORBA::Boolean     _verbose;
  
};
  


int main(int argc, char* const* argv) {

  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    orb->register_value_factory("IDL:Map/Point:1.0", 
                              (CORBA::ValueFactory)new PointFactory);
    orb->register_value_factory("IDL:Map/ConnectedPoint:1.0", 
                              (CORBA::ValueFactory)new ConnectedPointFactory);
    
    CORBA::Boolean Verbose = 0UL;
    // get a reference to the root POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var rootPOA = PortableServer::POA::_narrow(obj);

    CORBA::PolicyList policies;
    policies.length(1);

    policies[(CORBA::ULong)0] = rootPOA->create_lifespan_policy( 
                                                  PortableServer::PERSISTENT);
    // Create serverPOA with the right policies
    PortableServer::POAManager_var poa_manager = rootPOA->the_POAManager();
    PortableServer::POA_var serverPOA = rootPOA->create_POA("serverPoa",
                                                     poa_manager, policies); 

    for (int Index=1 ; Index < argc ; Index++) {
      if (strcmp( argv[Index], "-verbose") == 0)
        Verbose = 1UL;
    }

    PointManagerImpl pointManagerServant("PointManagerFactory", Verbose);

    // Decide on the ID for the servant
    PortableServer::ObjectId_var managerId = 
                           PortableServer::string_to_ObjectId("PointManager");

    // Activate the servant with the ID on serverPOA
    serverPOA->activate_object_with_id(managerId, &pointManagerServant);

    // Activate the POA Manager
    poa_manager->activate();

    cout << "PointManagerServant is ready" << endl; 

    // Wait for incoming requests
    orb->run();
  }
  catch(const CORBA::Exception& e) {
    cerr << e << endl;
    return 0;
  }
  return 1;
}
