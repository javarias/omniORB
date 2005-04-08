// Client.C

#include "map_c.hh"
#include "vport.h"
#include "PntImpl.h"

typedef char BUFFER[256];

// Command handler declarations
class commandDispatcher {
  public:
    struct CMD_TAB {
      const char * cmd;
      void (commandDispatcher::*handler)(void);
    };
  
    commandDispatcher(Map::PointManager_ptr pm) :_pm(pm),
      _x(0),_y(-1000) {}
  
    ~commandDispatcher(){}
  
    void dispatch(char * command, char * param) {
      _param = CORBA::string_dup(param ? param : "DUMMY");
      _label = new Map::Label(_param);
      if (command) {
        for (int i = 0; i < sizeof(_tab) / sizeof(CMD_TAB) ; i++) {
          if (VISPortable::vstricmp(command, _tab[i].cmd) == 0)
            (this->*_tab[i].handler)();
        }
      } else
        Usage();
    }
  
    void Usage() {
      cout << "USAGE INFORMATION" << endl;
      cout << "createbase <label>" << endl;
      cout << "createderived <label>" << endl;
      cout << "connect <label>" << endl;
      cout << "destroy <label>" << endl;
      cout << "get <label>" << endl;
      cout << "list <num copies of each>" << endl;
      cout << "quit" << endl;
      cout << "help/?" << endl << endl;
      cout << "if no label is given then \"DUMMY\""  
           << " is used as the default label" << endl << endl;
    }
  
    void createBase() {
      _point = _pm->create_point(_x++, _y++, _label, 0);
      displayPoint();
    }

    void createDerived() {
      _point = _pm->create_point(_x++, _y++, _label, 1); 
      displayPoint();
    }
  
    void Connect() {
      CORBA::Long count;
      BUFFER buffer = "";
      cout << "Connect " << _label << " To How many points?";
      cin.get(buffer, sizeof(BUFFER));
      cin.ignore();
      count = atoi(buffer);
  
      Map::LabelSeq seq;
      seq.length(count);
  
      for (CORBA::ULong i = 0; i < seq.length(); i++) {
        cout << " Connection #" << i << " :";
        cin.get(buffer, sizeof(BUFFER));
        cin.ignore();
        seq[i] = new Map::Label(CORBA::string_dup(buffer));
      }

      _pm->connect_point(_label, seq);
      cout << "Connected " << endl;
    }
  
    void Destroy() {  
      _pm->destroy_point(_label);  
    }
  
    void Get() {   
      _point = _pm->get_point(_label);
      displayPoint();
    }  
  
    // Obtains a list of points given the 
    // scaling factor
    void List() {
      Map::PointSeq_var seq = _pm->list_points(atoi(_param));
      for (CORBA::ULong i = 0; i < (*seq).length(); i++) {
        ((*seq)[i])->_add_ref();
        _point = (*seq)[i]; 
        if ( _point ) _point->print();
      }
      _point = (Map::Point_ptr)NULL;
    }
  
  private:
    Map::PointManager_var  _pm;
    CORBA::String_var      _param;  
    Map::Label_var         _label;
    CORBA::Long            _x, _y;
    Map::Point_var         _point;
    static CMD_TAB         _tab[9];
  
    void displayPoint() {
      if (_point)
        cout << "Label:" << _point->label() << " x:" << _point->x() << " y:" 
             << _point->y() << endl;
      else
        cout << "Point is Nil" << endl; 
    }
};

// Initialize Table 
commandDispatcher::CMD_TAB commandDispatcher::_tab[9] = {
  { "createbase", commandDispatcher::createBase },
  { "createderived", commandDispatcher::createDerived },
  { "connect",    commandDispatcher::Connect     },
  { "destroy",    commandDispatcher::Destroy     },
  { "get",        commandDispatcher::Get         },
  { "List",       commandDispatcher::List        },
  { "",           commandDispatcher::Usage       },
  { "Help",       commandDispatcher::Usage       },
  { "?",          commandDispatcher::Usage       },
};



// Start the music
int main(int argc, char* const* argv) {
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    BUFFER buffer = "";
    char seps[] = " \t";

    orb->register_value_factory("IDL:Map/Point:1.0", 
                                (CORBA::ValueFactory)new PointFactory);
    orb->register_value_factory("IDL:Map/ConnectedPoint:1.0", 
                                (CORBA::ValueFactory)new ConnectedPointFactory);

    // Get the manager Id
    PortableServer::ObjectId_var managerId = 
                            PortableServer::string_to_ObjectId("PointManager");

    // Locate an account manager. Give the full POA name and the servant ID.
    Map::PointManager_var manager = Map::PointManager::_bind("/serverPoa", 
                                                             managerId);

    CORBA::Boolean done = 0UL;
    char *command = NULL , *label = NULL;
    commandDispatcher cmd(Map::PointManager_var::_duplicate(manager));

    // Process Command
    while (!done) {
     try {
         cout << "Command-->";
         cin.get(buffer, sizeof(BUFFER));
         cin.ignore();
         command = strtok(buffer, seps);
         char * param  = command ? strtok(NULL, seps) : NULL;
         if (command && VISPortable::vstricmp(command, "quit") == 0) {
           cout << "Quitting..." << endl;
           done = 1UL;
           continue;
         }

         // dispatch
         cmd.dispatch(command, param);
     } catch(const Map::InvalidPoint& e) {
       cerr << "Invalid point exception Caught" << endl;
     } catch(const Map::DuplicatePoint& e) {
       cerr << "Duplicate point exception Caught" << endl;
     } catch(const Map::UnknownPoint& e) {
       cerr << "Unknown point exception Caught" << endl;
       if ( e.lab.in() ) cerr << e.lab << " - No such point" << endl;
     }
    }  

  } catch(const CORBA::Exception& e) {
    cerr << "Caught " << e << "Exception" << endl;
  }

  return 1;
}
