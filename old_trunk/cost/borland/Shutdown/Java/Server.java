import org.omg.PortableServer.*;
import com.inprise.vbroker.PortableServerExt.*;

public class Server extends Shutdown.ServerPOA {

  private org.omg.CORBA.ORB _orb;

  public Server(org.omg.CORBA.ORB orb) {
    _orb = orb;
  }

  public void shutdown(boolean sync) {
    _orb.shutdown(sync);
  }

  public void shutdown2(boolean sync) {
    _orb.shutdown(sync);
  }

  public static void main(String[] args) throws Exception {
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);

    // Reoslve root poa
    org.omg.PortableServer.POA rootPoa = org.omg.PortableServer.POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
    rootPoa.the_POAManager().activate();

    // Create a BindSupport Policy that makes POA register each servant
    // with osagent
    org.omg.CORBA.Any any = orb.create_any();
    BindSupportPolicyValueHelper.insert(any, BindSupportPolicyValue.BY_INSTANCE);

    org.omg.CORBA.Policy bsPolicy = orb.create_policy(BIND_SUPPORT_POLICY_TYPE.value, any);
    // Create policies for our testPOA
    org.omg.CORBA.Policy[] policies = {
      rootPoa.create_lifespan_policy
      (LifespanPolicyValue.PERSISTENT), bsPolicy
    };

    // Create managerPOA with the right policies
    org.omg.PortableServer.POA serverPoa = rootPoa.create_POA( "serverPoa", rootPoa.the_POAManager(), policies );


    serverPoa.activate_object_with_id("Shutdown Server".getBytes(), new Server(orb));
    System.out.println("Server is ready.");
    orb.run();
    System.out.println("Server shutdown.");
  }

}
