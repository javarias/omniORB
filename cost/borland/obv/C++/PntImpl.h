#ifndef __PTIMPL_INCLUDE__
#define __PTIMPL_INCLUDE__

// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

// Point Implementation
class PointImpl : public virtual Map::OBV_Point,
                  public virtual CORBA::DefaultValueRefCountBase
{
  public:
    PointImpl(){}
    virtual ~PointImpl(){}
    CORBA_ValueBase* _copy_value() { 
      return new PointImpl(x(), y(), 
                           new Map::Label(CORBA::string_dup(label()))); 
    }
    virtual void print() {
      cout << "Point is [" << label() << ": (" 
           << x() << ", " << y() << ")]" << endl << endl;
    } 
    PointImpl( CORBA::Long x, CORBA::Long y, Map::Label_ptr label ) 
       : OBV_Point( x,y,label->_boxed_in()) 
    {}
};



// ConnectedPoint Implementation
class ConnectedPointImpl : public virtual Map::OBV_ConnectedPoint,
                           public virtual PointImpl
{
  public:
    ConnectedPointImpl(){}

    ConnectedPointImpl(CORBA::Long x, 
                       CORBA::Long y, 
                       Map::Label_ptr label,
                       Map::PointSeq connectedPoints)
       : OBV_ConnectedPoint( connectedPoints ), 
         OBV_Point( x,y,label->_boxed_in()) 
    {}

    virtual ~ConnectedPointImpl() {}

    CORBA::ValueBase* _copy_value() {
#if defined(MSVCNESTEDBASE_BUG)
      return new ConnectedPointImpl(OBV_ConnectedPoint::x(), 
               OBV_ConnectedPoint::y(),
               new Map::Label(OBV_ConnectedPoint::label()), connected_points());
#else
      return new ConnectedPointImpl(Map::OBV_ConnectedPoint::x(), 
               Map::OBV_ConnectedPoint::y(),
               new Map::Label(Map::OBV_ConnectedPoint::label()), 
                              connected_points());
#endif
    }

    virtual void print() {
#if defined(MSVCNESTEDBASE_BUG)
      cout << "Derived Point is [" << OBV_ConnectedPoint::label() << ": ("
           << OBV_ConnectedPoint::x() << ", " << OBV_ConnectedPoint::y() 
           << ")]" << endl;
#else
      cout << "Derived Point is [" << Map::OBV_ConnectedPoint::label() 
           << ": (" << Map::OBV_ConnectedPoint::x() << ", " 
           << Map::OBV_ConnectedPoint::y() << ")]" << endl;
#endif
      Map::PointSeq& conn_pts = connected_points();
      CORBA::ULong ul = conn_pts.length();
      if (ul){ 
        cout << "Connected to "; 
        for (;ul;) {
          cout << conn_pts[--ul]->label();
          if (ul > 0) cout << ",";
        }
        cout << endl; 
      }
       cout << endl; 
    }

    // add a new point to connection
    void add_connection(Map::Point_ptr _p) {
      Map::PointSeq& conn_pts = connected_points();
      conn_pts.length( conn_pts.length() + 1 );
      CORBA::add_ref(_p);
      conn_pts[ conn_pts.length() - 1 ] = _p;
    }
};

// Point Factory
class PointFactory: public CORBA::ValueFactoryBase 
{
  public:
    PointFactory(){}
    virtual  ~PointFactory(){}

    CORBA::ValueBase*  create_for_unmarshal() {
      return new PointImpl();
    }
};

// ConnectedPoint Factory
class ConnectedPointFactory: public CORBA::ValueFactoryBase 
{
  public:
    ConnectedPointFactory(){}
    virtual  ~ConnectedPointFactory(){}

    CORBA::ValueBase * create_for_unmarshal() {
      return new ConnectedPointImpl();
    }
};


#endif // __PTIMPL_INCLUDE__ 
