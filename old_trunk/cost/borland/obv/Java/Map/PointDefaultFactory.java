package Map;


/**
 * <ul>
 * <li> <b>IDL Source</b>    "map.idl"
 * <li> <b>IDL Name</b>      ::Map::Point
 * <li> <b>Repository Id</b> IDL:Map/Point:1.0
 * </ul>
 * <b>IDL definition:</b>
 * <pre>
 * valuetype Point  {
  ...
};
 * </pre>
 */
public class PointDefaultFactory implements org.omg.CORBA.portable.ValueFactory {
  public java.io.Serializable read_value (org.omg.CORBA.portable.InputStream is) {
    java.io.Serializable val = new PointImpl();
    // create and initialize value
    val = ((org.omg.CORBA_2_3.portable.InputStream)is).read_value(val);
    return val;
  }
}
