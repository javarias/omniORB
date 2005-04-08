import com.inprise.vbroker.orb.ORB;
import com.inprise.vbroker.ServerManager.*;
import com.inprise.vbroker.ServerManager.ContainerPackage.*;

public class ShutdownClient {
  public static void main(String[] args) {
    Container orbMgr = null;

    try {
      ORB orb = (ORB) org.omg.CORBA.ORB.init(args,null);

      if (args.length == 0) {
        System.out.println("FAIL - Container name not specified");
        System.exit(1);
      }
      else {
        System.out.println("Container name specified is " + args[0]);
        System.out.println("Directly binding to the container");
        orbMgr = ContainerHelper.bind(orb, args[0]);
      }

      // Shut down the server
      System.out.println("Shutting down the server ...");
      Operation sh = null;
      try {
        NamedContainer orbNamedCtnr = orbMgr.get_container("ORB");
        Container orbCtnr = orbNamedCtnr.value;

        sh = orbCtnr.get_operation("shutdown");
        //System.out.println(sh);

        //Change the wait_for_completion parameter to false (default is true)
        org.omg.CORBA.Any wait_param = sh.params[0].value;
        wait_param.insert_boolean(false);
        sh.params[0].value = wait_param;
        //System.out.println(sh);

        Parameter[] params = sh.params;
        org.omg.CORBA.TypeCode result = sh.result;
        sh = new Operation("shutdown", params, result);

        org.omg.CORBA.Any reslt = orbCtnr.do_operation(sh);
        System.out.println("Server shut down successfully");
      } catch (org.omg.CORBA.MARSHAL e) {
        System.out.println("Expected MARSHAL exception from C++ server caught: " + e);
      } catch (Exception e) {
        System.out.println("shutdown() invocation failed with exception " + e);
      }
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
