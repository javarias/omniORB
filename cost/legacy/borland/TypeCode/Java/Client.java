
import org.omg.CORBA.TypeCode;
import TypeCodeTest.*;

public class Client {

  private static Echo echo;

  public static void main(String[] args) {
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);


    if (args.length == 1) {
      echo = EchoHelper.narrow(orb.string_to_object(args[0]));
    }
    else {
      echo = EchoHelper.bind(orb);
    }
    /*
    //testSeqRecur(orb);
    //testValueRecursionSimple(orb);

    testTypeCode(RecursiveStructTestHelper.type());
    System.exit(0);
    */
    // primitive
    testTypeCode(orb.get_primitive_tc(org.omg.CORBA.TCKind.tk_long));

    // objref
    testTypeCode(EchoHelper.type());

    // struct, string, wstring, array, sequence
    testTypeCode(SimpleStructTestHelper.type());
    
    // enum
    testTypeCode(EnumTestHelper.type());
    
    // union
    testTypeCode(SimpleUnionTestHelper.type());
    testTypeCode(DefaultUnionTestHelper.type());
    testTypeCode(EnumUnionTestHelper.type());

    // sequence of sequemce of sequence of simple type
    testTypeCode(DamsonsHelper.type());

    // recursive struct
    testTypeCode(RecursiveStructTestHelper.type());

    // Hand crafted recur seq to follow new style
    testSeqRecur(orb);    

    testTypeCode(NestedRecursiveStructHelper.type());
    testTypeCode(NestedRecursiveStructListHelper.type());
    testTypeCode(RecursiveStructTestListHelper.type());
    testTypeCode(ComplexStructTestHelper.type());
    testTypeCode(ComplexUnionTestHelper.type());
    testTypeCode(ComplexUnionTestListHelper.type());

    // Until compiler is ready test it the hard way
    testValueType(orb);

    testValueRecursionSimple(orb);
  }

  public static void testValueType(org.omg.CORBA.ORB orb) {
    org.omg.CORBA.TypeCode tc1;
    org.omg.CORBA.ValueMember[] smember1 = new org.omg.CORBA.ValueMember[2];
    
    smember1[0] = new org.omg.CORBA.ValueMember("name11", "id11",
"def11", "ver11", orb.create_interface_tc("IDL:MyInter", "SIMBA1"), null, org.omg.CORBA.PRIVATE_MEMBER.value);
    smember1[1] = new org.omg.CORBA.ValueMember("name12", "id12",
"def12", "ver12", orb.create_interface_tc("IDL:MyInter", "SIMBA1"), null, org.omg.CORBA.PRIVATE_MEMBER.value);


    tc1 = orb.create_value_tc("IDL:MyValue", "MOUGLI", org.omg.CORBA.VM_NONE.value, orb.get_primitive_tc(org.omg.CORBA.TCKind.tk_long), smember1);

    testTypeCode(tc1);
  }


  public static void testValueRecursionSimple(org.omg.CORBA.ORB orb) {
    org.omg.CORBA.TypeCode tc1;
    org.omg.CORBA.TypeCode recur_tc1;

    recur_tc1 = orb.create_recursive_tc("IDL:MyValue");
    org.omg.CORBA.ValueMember[] smember1 = new org.omg.CORBA.ValueMember[1];
    smember1[0] = new org.omg.CORBA.ValueMember("name11", "id11",
"def11", "ver11", recur_tc1, null, org.omg.CORBA.PRIVATE_MEMBER.value);
    
    tc1 = orb.create_value_tc("IDL:MyValue", "MOUGLI", org.omg.CORBA.VM_NONE.value, orb.get_primitive_tc(org.omg.CORBA.TCKind.tk_long), smember1);
    testTypeCode(tc1);
  }




  public static void testSeqRecur(org.omg.CORBA.ORB orb) {
    org.omg.CORBA.TypeCode tc1;
    org.omg.CORBA.TypeCode recur_tc1;
    org.omg.CORBA.TypeCode seq_tc;

    recur_tc1 = orb.create_recursive_tc("IDL:MyStruct");
    seq_tc = orb.create_sequence_tc(100, recur_tc1);
    org.omg.CORBA.StructMember[] smember1 = new org.omg.CORBA.StructMember[1];
    smember1[0] = new org.omg.CORBA.StructMember("name11", seq_tc, null);
    
    tc1 = orb.create_struct_tc("IDL:MyStruct", "MOUGLI", smember1);
    testTypeCode(tc1);
  }



  public static void testTypeCode(TypeCode tc) {
    System.out.println("Testing: " + tc);
    TypeCode result = echo.doEcho(tc);
    if (!result.equal(tc)) {
      System.out.println("Unequal TypeCodes: " + tc + " and " + result);
    } else {
      System.out.println("Equal Typecodes: " + tc + " and " + result);
    }
  }

}
