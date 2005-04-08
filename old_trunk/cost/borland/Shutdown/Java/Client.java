
public class Client {

  public static void main(String[] args) {
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);

    Shutdown.Server server = Shutdown.ServerHelper.bind(orb);

    if (args.length == 1) {
      if(args[0].equals("oneway")) {
	server.shutdown(false);
	System.out.println("Test succeeded!");
      }
      else if(args[0].equals("sync")) {
	try{
	  server.shutdown2(true);
	}
	catch(org.omg.CORBA.BAD_INV_ORDER e) {
	  System.out.println("Test succeeded!");
	  System.exit(0);
	}
	System.err.println("Test failed!");
      }
      else if(args[0].equals("async")) {
	server.shutdown2(false);
	System.out.println("Test succeeded!");
      }
      else {
	System.err.println("Usage: Client (oneway | sync | async)");
      }
    } else {
      System.err.println("Usage: Client (oneway | sync | async)");
    }
  }
}
