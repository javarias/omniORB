package Map;


/**
 * <ul>
 * <li> <b>IDL Source</b>    "map.idl"
 * <li> <b>IDL Name</b>      ::Map::ConnectedPoint
 * <li> <b>Repository Id</b> IDL:Map/ConnectedPoint:1.0
 * </ul>
 * <b>IDL definition:</b>
 * <pre>
 * valuetype ConnectedPoint : Map.Point {
  ...
};
 * </pre>
 */
public class ConnectedPointDefaultFactory implements org.omg.CORBA.portable.ValueFactory {
  public java.io.Serializable read_value (org.omg.CORBA.portable.InputStream is) {
    java.io.Serializable val = new ConnectedPointImpl();
    // create and initialize value
    val = ((org.omg.CORBA_2_3.portable.InputStream)is).read_value(val);
    return val;
  }
}
