
import java.util.*;
import extest.*;
import extest.TestPackage.*;

public class Client {
  
  public static void main (String args[])  {
     
    System.out.println ("Initalizing the ORB");
    org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
    System.out.println ("Binding to the ORB");
     
    Test wcharobj =extest.TestHelper.bind (orb,"Extended Types");
    char c;
     
    //System.out.println ("Making Remote Invoccation");
    // invoke the t_wc method
    try {
      c = wcharobj.t_wc();
      //System.out.println (" The value returned from t_wc is " + c);
      if (c=='\u00EA')
	System.out.println (" Testing wchar..........passed.");
      else  System.out.println (" Testing wchar..........failed. ");}
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }
     
    // invoke the t_wstr method
    String str;
    try{
      str= wcharobj.t_wstr();
      //System.out.println (" The value returned from t_wstr is " + str);
      if (str.equals("Hello World"))
	System.out.println (" Testing wstring..........passed.");
      else  System.out.println (" Testing wstring..........failed. ");}
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }
     
    // invoke t_ll() method
    long lint;
    try{
      lint = wcharobj.t_ll();
      //System.out.println (" The value returned from t_ll is " + lint);
      if (lint==  986532235689L )
	System.out.println (" Testing longlong..........passed.");
      else  System.out.println (" Testing longlong..........failed. ");}
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }
     
    // invoke t_ull() method
    long luint;
    try {
      luint = wcharobj.t_ull();
      //System.out.println (" The value returned from t_ull is " + luint);
      if (luint== 987654321L )
	System.out.println (" Testing ulonglong..........passed.");
      else  System.out.println (" Testing ulonglong..........failed. ");}
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }
     
    // invoke t_wc_inout method
    org.omg.CORBA.CharHolder ch = new org.omg.CORBA.CharHolder('\u009A');
    //System.out.println (" The value sent is t_wc_inout " + ch.value );
    try{
      wcharobj.t_wc_inout(ch);
      //System.out.println (" The value returned is t_wc_inout " + ch.value );
      if (ch.value == '\u00BA')
	System.out.println (" Testing wchar inout..........passed.");
      else  System.out.println (" Testing wchar inout..........failed. ");}
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }
     
    //invoke t_wstr_inout method
    str ="Hello World";
    try{
      org.omg.CORBA.StringHolder strhr = new org.omg.CORBA.StringHolder ( str);
      //System.out.println (" The value sent is t_wstr_inout " + strhr.value );
      wcharobj.t_wstr_inout(strhr);
      str = strhr.value;
      //System.out.println (" The value returned from t_wstr_inout is " + str);
      if (str.equals("Hello to you too."))
	System.out.println (" Testing wstring inout..........passed.");
      else  System.out.println (" Testing wstring inout..........failed. ");}
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }
     
    //invoke t_ll_inout method
    lint= 986532235689L;
    try{
      org.omg.CORBA.LongHolder llhr = new org.omg.CORBA.LongHolder ( lint);
      //System.out.println (" The value sent is t_ll_inout " + llhr.value );
      wcharobj.t_ll_inout(llhr);
      lint = llhr.value;
      //System.out.println (" The value returned from t_ll_inout is " + lint);
      if (lint==9223372036854775807L)
	System.out.println (" Testing longlong inout..........passed.");
      else  System.out.println (" Testing longlong inout..........failed. ");}
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }
     
    //invoke t_ull_inout method
    luint = 987654321L;
    try{
      org.omg.CORBA.LongHolder lluhr = new org.omg.CORBA.LongHolder ( luint);
      //System.out.println (" The value sent is t_ll_inout " + lluhr.value );
      wcharobj.t_ull_inout(lluhr);
      luint = lluhr.value;
      //System.out.println (" The value returned from t_ull_inout is " + luint);
      if (luint == (-9223372036854775807L))
	System.out.println (" Testing ulonglong inout..........passed.");
      else  System.out.println (" Testing ulonglong inout..........failed. ");}
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }
     
    //invoke t_struct method
    extS st = new extS() ;
    try{
      st = wcharobj.t_struct();
      //System.out.println (" The value returned from t_struct is " + st.twc +" "+st.twstr+" "+st.tll+" "+st.tull+"." );
      if ((st.twc == 'A' ) && (st.twstr.equals("hi there"))&& (st.tll==123456789L ) && (st.tull==-123456789L ))
        System.out.println (" Testing struct..........passed..");
        
      else  System.out.println (" Testing struct..........failed. "); }
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }
    
    //invoke t_struct_inout method
    try{
      extest.TestPackage.extSHolder sthr = new extest.TestPackage.extSHolder ( st);
      //System.out.println (" The value sent in t_struct_inout is " + st.twc +" "+st.twstr+" "+st.tll+" "+st.tull+"." );
      wcharobj.t_struct_inout(sthr);
      st=sthr.value;
      //System.out.println (" The value returned from t_struct_inout is " + st.twc +" "+st.twstr+" "+st.tll+" "+st.tull+"." );
      if ((st.twc == 'Z' ) && (st.twstr.equals("this is right"))&& (st.tll==987654321L ) && (st.tull==-987654321L ))
        System.out.println (" Testing struct inout..........passed..");
        
      else  System.out.println (" Testing struct inout..........failed. ");}
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }
    
    
    //invoke t_extTS_inout method
     
    extS[] stt = new extS[5];
     
    int i, n;
    n=5;
    try{
      for ( i=0;i<n;i++){
	stt[i]=wcharobj.t_struct();
	//System.out.println (i+" The value return from t_struct is " + stt[i].twc +" "+stt[i].twstr+" "+stt[i].tll+" "+stt[i].tull+"." );
      }
     
      extest.TestPackage.extTestSeqHolder stthr = new extest.TestPackage.extTestSeqHolder(stt);
      wcharobj.t_extTS_inout(stthr);
      stt=stthr.value;
      n=stt.length;
     
      for (i=0;i<n;i++){
	//System.out.println (i+" The value returned from t_struct is " + stt[i].twc +" "+stt[i].twstr+" "+stt[i].tll+" "+stt[i].tull+"." );
	if ((stt[i].twc == 'Z' ) && (stt[i].twstr.equals("this is right"))&& (stt[i].tll==987654321L ) && (stt[i].tull==-987654321L ))
	  System.out.println (i+" Testing sequence..........passed..");
        
	else  System.out.println (i+" Testing sequence..........failed. ");}}
    catch (org.omg.CORBA.SystemException e){
      System.out.println(e);
    }
     
  }


}
