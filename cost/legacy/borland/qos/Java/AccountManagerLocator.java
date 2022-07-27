import org.omg.PortableServer.*;
import org.omg.PortableServer.ServantLocatorPackage.CookieHolder;

public class AccountManagerLocator extends ServantLocatorPOA {
  Servant servant;
  org.omg.CORBA.Object object;
  int i=-1;
  
  public Servant preinvoke (byte[] oid,POA adapter,
                            java.lang.String operation,
                            CookieHolder the_cookie) throws ForwardRequest {
    ++i;
    if(servant==null) {
      servant = (Servant )new AccountManagerImpl();
    }
    
    if(i%2==1) {
      if(object==null) {
	object= servant._this_object();
      }
      System.out.println("Server: throwing ForwardRequest");
      
      throw new ForwardRequest(object);
    }
    
    return servant;
   }

  public void postinvoke (byte[] oid,
                          POA adapter,
                          java.lang.String operation,
                          java.lang.Object the_cookie,
                          Servant the_servant) {
    }

}
