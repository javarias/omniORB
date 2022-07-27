import org.omg.CORBA.ORB;
import org.omg.CORBA.Policy;
import org.omg.PortableServer.*;
import com.inprise.vbroker.PortableServerExt.BindSupportPolicyValue;
import com.inprise.vbroker.PortableServerExt.BindSupportPolicyValueHelper;
import com.inprise.vbroker.PortableServerExt.BIND_SUPPORT_POLICY_TYPE;

public class Server extends TypeCodeTest.EchoPOA {

  Server(String name) {

  }

  public static void main(String[] args) {
    try {
      // Initialize the ORB.
      ORB orb = ORB.init(args,null);
      // get a reference to the root POA
      POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));

      // Create policies for our persistent POA
      org.omg.CORBA.Any reg_value = orb.create_any();
      BindSupportPolicyValueHelper.insert(reg_value,BindSupportPolicyValue.BY_INSTANCE);
      Policy register_all_objects = orb.create_policy(BIND_SUPPORT_POLICY_TYPE.value, reg_value);

      Policy persistent_lifespan = rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT);

      org.omg.CORBA.Policy[] policies = { 
        persistent_lifespan,
        register_all_objects
      };
      // Create myPOA with the right policies
      POA myPOA = rootPOA.create_POA("typecode_marshal_server_agent_poa", rootPOA.the_POAManager(), 
                                     policies );

      String name = "Typecode Marshal Test";
      Server servant = new Server(name);

      myPOA.activate_object_with_id(name.getBytes(), servant);
      rootPOA.the_POAManager().activate();
      System.out.println(myPOA.servant_to_reference(servant) + " is ready");

      // wait for incoming requests.
      orb.run();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  public org.omg.CORBA.TypeCode doEcho(org.omg.CORBA.TypeCode t) {
    System.out.println("Incoming typecode = " + t);
    return t;
  }
}
