// Client.java

import org.omg.DynamicAny.*;


public class Client {

  public static void main(String[] args) {
    try {
      // Initialize the ORB.
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);

      DynAnyFactory factory = DynAnyFactoryHelper.narrow(orb.resolve_initial_references("DynAnyFactory"));

      // Locate a printer manager.
      Printer.PrinterManager manager = 
	Printer.PrinterManagerHelper.bind(orb, "/serverPoa", "PrinterManager".getBytes());

      // Create Dynamic struct
      DynStruct info = DynStructHelper.narrow(factory.create_dyn_any_from_type_code(Printer.StructTypeHelper.type()));

      // Create our NameValuePair sequence (array)
      NameValuePair[] NVPair = new NameValuePair[3];

      // Create and initialize Dynamic Struct data as any's
      org.omg.CORBA.Any str_any = orb.create_any();
      str_any.insert_string("String");
      org.omg.CORBA.Any e_any = orb.create_any();
      Printer.EnumTypeHelper.insert(e_any, Printer.EnumType.second);
      org.omg.CORBA.Any fl_any = orb.create_any();
      fl_any.insert_float((float)864.50);

      NVPair[0] = new NameValuePair("str", str_any);
      NVPair[1] = new NameValuePair("e", e_any);
      NVPair[2] = new NameValuePair("fl", fl_any);

      // Initialize the Dynamic Struct
      info.set_members(NVPair);

      manager.printAny(info.to_any());

      manager.shutdown();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }

}
