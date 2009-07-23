import java.util.*;
import extest.*;
import extest.TestPackage.*;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;
import org.omg.CORBA.Policy;
import org.omg.PortableServer.IdUniquenessPolicyValue;
import org.omg.PortableServer.IdAssignmentPolicyValue;
import org.omg.PortableServer.RequestProcessingPolicyValue;
import org.omg.PortableServer.ServantRetentionPolicyValue;
import org.omg.PortableServer.LifespanPolicyValue;
// import com.inprise.vbroker.PortableServer.BindSupportPolicyValue;
// import com.inprise.vbroker.PortableServer.BIND_SUPPORT_POLICY_TYPE;
// import com.inprise.vbroker.PortableServer.BindSupportPolicyValueHelper;
import com.inprise.vbroker.PortableServerExt.*;


class Server extends extest.TestPOA {

  public char t_wc() {
    return '\u00EA';
  }

  public java.lang.String t_wstr(){
    return ("Hello World");
  }

  public long t_ll(){
    return 986532235689L;

  }

  public long t_ull(){
    return 987654321L;
  }

  public void t_wc_inout (org.omg.CORBA.CharHolder twc)
  {  
   
    if (twc.value=='\u009A')
      twc.value='\u00BA' ;
    else twc.value='\u00A0';
  }

  public void t_wstr_inout(org.omg.CORBA.StringHolder twstr)
  {
    String str;
    str=twstr.value;
    if (str.equals("Hello World"))
      twstr.value = "Hello to you too.";
    else twstr.value = "Bad String Sent";
   
  }

  public void t_ll_inout(org.omg.CORBA.LongHolder tll)
  {
    if (tll.value == 986532235689L)
      tll.value = 9223372036854775807L;
    else tll.value = 123L;
  }
  public void t_ull_inout(org.omg.CORBA.LongHolder tull)
  {
    if (tull.value == 987654321L)
      tull.value = -9223372036854775807L;
    else tull.value = -123L;
  }

  public extest.TestPackage.extS t_struct() 
  {
    extS t= new extS ('A',"hi there",123456789L,-123456789L);
    return (t);
  }

  public void t_struct_inout(extest.TestPackage.extSHolder teststruct)
  {
    extS x;
    x = teststruct.value;
    
    if (x.twc == 'A')
      x.twc='Z';
    else x.twc='9';
                                                        
    if (x.twstr.equals("hi there"))
      x.twstr = "this is right";
    else x.twstr="this is wrong";
      
    if (x.tll == 123456789L)
      x.tll=987654321L;
    else x.tll = 123L;
       
    if (x.tull == -123456789L)
      x.tull = -987654321L;
    else x.tull = -123L;
       
    teststruct.value=x;      
  }

  public void t_extTS_inout(extest.TestPackage.extTestSeqHolder testseq) {
    int n;
      
    int i;
      
    n=testseq.value.length;
     
    extS[] x = new extS[n];
    x = testseq.value;
      
    if (n > 0)
      { for ( i=0; i<n;i++)
	{   if (x[i].twc == 'A')
	  x[i].twc='Z';
	else x[i].twc='9';
                                                        
	if (x[i].twstr.equals("hi there"))
	  x[i].twstr = "this is right";
	else x[i].twstr="this is wrong";
      
	if (x[i].tll == 123456789L)
	  x[i].tll=987654321L;
	else x[i].tll = 123L;
       
	if (x[i].tull == -123456789L)
	  x[i].tull = -987654321L;
	else x[i].tull = -123L;
	}
           
      }
    testseq.value=x;
  
  }  
  public static void main (String[] args) 
  {
    try {
      org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
      POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
      Policy[] policies = new Policy[6];

    // get the correct policies for the IRPOA
      policies[0] = rootPOA.create_id_uniqueness_policy(IdUniquenessPolicyValue.UNIQUE_ID);
      policies[1] = rootPOA.create_id_assignment_policy(IdAssignmentPolicyValue.USER_ID);
      policies[2] = rootPOA.create_request_processing_policy(RequestProcessingPolicyValue.USE_ACTIVE_OBJECT_MAP_ONLY);
      policies[3] = rootPOA.create_servant_retention_policy(ServantRetentionPolicyValue.RETAIN);
      policies[4] = rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT);
      org.omg.CORBA.Any any = orb.create_any();
      BindSupportPolicyValueHelper.insert(any, BindSupportPolicyValue.BY_INSTANCE);
      try {
        policies[5] = ((com.inprise.vbroker.orb.ORB)orb).create_policy(BIND_SUPPORT_POLICY_TYPE.value, any);
      } catch (org.omg.CORBA.PolicyError e) {
        // Can't happen
      }
      POA sPoa = rootPOA.create_POA("Extended Types", rootPOA.the_POAManager(), policies);
      System.out.println("Initialised the ORB and POA");
      sPoa.activate_object_with_id("Extended Types".getBytes(), new Server());
      sPoa.the_POAManager().activate();
      System.out.println ("Server is ready.");
      orb.run();
    }
    catch (org.omg.CORBA.ORBPackage.InvalidName e){
      // from resolve_initial_references, Cannot happen
      System.out.println(e);
    }
    catch (org.omg.PortableServer.POAPackage.AdapterAlreadyExists e){
      // from create_POA, Cannot happen
      System.out.println(e);
    }
    catch (org.omg.PortableServer.POAPackage.InvalidPolicy e){
      // from create_POA, happen
      System.out.println(e);
    }
    catch (org.omg.PortableServer.POAPackage.ServantAlreadyActive e){
      // from activate_object_with_id, Cannot happen
      System.out.println(e);
    }
    catch (org.omg.PortableServer.POAPackage.ObjectAlreadyActive e){
      // from activate_object_with_id, Cannot happen
      System.out.println(e);
    }
    catch (org.omg.PortableServer.POAPackage.WrongPolicy e){
      // from activate_object_with_id, Cannot happen
      System.out.println(e);
    }
    catch (org.omg.PortableServer.POAManagerPackage.AdapterInactive e){
      // from theManager().activate, Cannot happen
      System.out.println(e);
    }
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }

  }


}
