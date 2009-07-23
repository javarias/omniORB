// PrinterManagerImpl.java

import java.util.*;
import org.omg.DynamicAny.*;
import org.omg.PortableServer.*;

public class PrinterManagerImpl extends Printer.PrinterManagerPOA {
  private com.inprise.vbroker.CORBA.ORB _orb;
  private DynAnyFactory _factory;
  private POA _poa;
  private byte[] _oid;

  public PrinterManagerImpl(com.inprise.vbroker.CORBA.ORB orb, DynAnyFactory factory, POA poa, byte[] oid) {
    _orb = orb;
    _factory = factory;
    _poa = poa;
    _oid = oid;
  }

  public synchronized void printAny(org.omg.CORBA.Any info) {
    // Display info with the assumption that we don't have
    // any info statically about the type inside the any 

    try {
      // Create a DynAny object
      DynAny dynAny = _factory.create_dyn_any(info); 
      display(dynAny);
    }
    catch (Exception e) {
      e.printStackTrace();
    }
    
  }

  public void shutdown() {
    try {
      _poa.deactivate_object(_oid); 
      System.out.println("Server shutting down");
      _orb.shutdown(false);
    }
    catch (Exception e) {
      System.out.println(e);
    }
  }

  private void display(DynAny value) throws Exception {
    switch(value.type().kind().value()) {
      case org.omg.CORBA.TCKind._tk_null:
      case org.omg.CORBA.TCKind._tk_void: {
       break;
      }
      case org.omg.CORBA.TCKind._tk_short: {
        System.out.println(value.get_short());
        break;
      }
      case org.omg.CORBA.TCKind._tk_ushort: {
        System.out.println(value.get_ushort());
        break;
      }
      case org.omg.CORBA.TCKind._tk_long: {
        System.out.println(value.get_long());
        break;
      }
      case org.omg.CORBA.TCKind._tk_ulong: {
        System.out.println(value.get_ulong());
        break;
      }
      case org.omg.CORBA.TCKind._tk_float: {
        System.out.println(value.get_float());
        break;
      }
      case org.omg.CORBA.TCKind._tk_double: {
        System.out.println(value.get_double());
        break;
      }
      case org.omg.CORBA.TCKind._tk_boolean: {
        System.out.println(value.get_boolean());
        break;
      }
      case org.omg.CORBA.TCKind._tk_char: {
        System.out.println(value.get_char());
        break;
      }
      case org.omg.CORBA.TCKind._tk_octet: {
        System.out.println(value.get_octet());
        break;
      }
      case org.omg.CORBA.TCKind._tk_string: {
        System.out.println(value.get_string());
        break;
      }
      case org.omg.CORBA.TCKind._tk_any: {
        DynAny dynAny = _factory.create_dyn_any(value.get_any()); 
        display(dynAny);
        break;
      }
      case org.omg.CORBA.TCKind._tk_TypeCode: {
        System.out.println(value.get_typecode());
        break;
      }
      case org.omg.CORBA.TCKind._tk_objref: {
        System.out.println(value.get_reference());
        break;
      }
      case org.omg.CORBA.TCKind._tk_enum: {
        DynEnum dynEnum = DynEnumHelper.narrow(value);
        System.out.println(dynEnum.get_as_string());
        break;
      }
      case org.omg.CORBA.TCKind._tk_union: {
        DynUnion dynUnion = DynUnionHelper.narrow(value);
        display(dynUnion.get_discriminator());
        display(dynUnion.member());
        break;
      }
      case org.omg.CORBA.TCKind._tk_struct:
      case org.omg.CORBA.TCKind._tk_array:
      case org.omg.CORBA.TCKind._tk_sequence: {
        value.rewind();
        boolean next = true; 
        while(next) {
          DynAny d = value.current_component();
          display(d);
          next = value.next();
        }
        break;
      }
      case org.omg.CORBA.TCKind._tk_longlong: {
        System.out.println(value.get_longlong());
        break;
      }
      case org.omg.CORBA.TCKind._tk_ulonglong: {
        System.out.println(value.get_ulonglong());
        break;
      }
      case org.omg.CORBA.TCKind._tk_wstring: {
        System.out.println(value.get_wstring());
        break;
      }
      case org.omg.CORBA.TCKind._tk_wchar: {
        System.out.println(value.get_wchar());
        break;
      }
      default:
        System.out.println("Invalid type");
      
    }
  }
}

