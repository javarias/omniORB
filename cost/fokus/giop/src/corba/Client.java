/******************************************************************************
*
*  Copyright (c) 2002                                               
*  FhG FOKUS                                                        
*  Kaiserin-Augusta-Allee 31                                       
*  10589 Berlin                                                  
*  Germany                                                     
*  Tel: +49-30-3463-7000                 
*  Fax: +49-30-3463-8000
*
*  Java implementation of a CORBA client.
*
*****************************************************************************/

package transferdata;
import org.omg.CORBA.*;
import transferdata.TransferDataPackage.*;
import java.io.*;

/**
 * This Java class implements the CORBA client which can be used to
 * invoke methods of the CORBA server which is used as test component
 * for the TTCN-3 based GIOP/IIOP tests.
 */
public class Client
{
    /**
     * 'main' initialises the ORB and runs the client.
     *
     * @param args[] command line arguments
     */
    public static void main(String args[])
    {
        java.util.Properties props = System.getProperties();
        props.put("org.omg.CORBA.ORBClass", "com.ooc.CORBA.ORB");
        props.put("org.omg.CORBA.ORBSingletonClass",
                  "com.ooc.CORBA.ORBSingleton");
        int status = 0;
        org.omg.CORBA.ORB orb = null;
        
        try
        {
            orb = org.omg.CORBA.ORB.init(args, props);
            status = run(orb);
        }
        catch(Exception ex)
        {
            ex.printStackTrace();
            status = 1;
        }
        if (orb != null)
        {
            try
            {
                ((com.ooc.CORBA.ORB) orb).destroy();
            }
            catch(Exception ex)
            {
                ex.printStackTrace();
                status = 1;
            }
        }
        System.exit(status);
    }
    
    /**
     * 'run' is the actual CORBA client.
     *
     * @param orb the ORB object of type 'org.omg.CORBA.ORB'
     * @return zero if all is OK, non-zero otherwise
     */
    static int run(org.omg.CORBA.ORB orb)
    {
        int c = 0;

        org.omg.CORBA.Object obj = null;
        try
        {
            // read IOR
            String refFile = "TransferData.ref";
            java.io.BufferedReader in = new java.io.BufferedReader
                (new java.io.FileReader(refFile));
            String ref = in.readLine();
            obj = orb.string_to_object(ref);
        }
        catch(java.io.IOException ex)
        {
            ex.printStackTrace();
            return 1;
        }
        TransferData transferdata = TransferDataHelper.narrow(obj);
        try 
        {
            BufferedReader infile =
                new BufferedReader (new InputStreamReader (System.in));
            System.out.println("Press 'Return' for invocation or 'q' for exit:");
            c = infile.read();
        } catch (Exception e)
        {
            System.out.println("wrong input: - " + e);
        }
        if (c!= 'q')
        {
            System.out.println("------Basic Types------");

            System.out.println("boolean: " + transferdata.return_boolean());
            System.out.println("char: " + transferdata.return_char());
            System.out.println("string: " + transferdata.return_string());
            System.out.println("wstring: " + transferdata.return_wstring());
            System.out.println("octet: " + transferdata.return_octet());
            System.out.println("short: " + transferdata.return_short());
            System.out.println("unsigned short: " +
                               transferdata.return_ushort());
            System.out.println("long: " + transferdata.return_long());
            System.out.println("unsigned long: " +
                               transferdata.return_ulong());
            System.out.println("long long: " + transferdata.return_longlong());
            System.out.println("unsigned long long: " +
                               transferdata.return_ulonglong());
            System.out.println("float: " + transferdata.return_float());
            System.out.println("double: " + transferdata.return_double());
            System.out.println("fixed: " + transferdata.return_fixed());
 
            System.out.println("------Constructed Types------");

            switch (transferdata.return_enum().value())
            {
                case Colour._red: System.out.println ("enum: red");
                    break;
                case Colour._green: System.out.println ("enum: green");
                    break;
                case Colour._blue: System.out.println ("enum: blue");
                    break;
                default: System.out.println ("enum: strange colour!");
                    break;
            }
            System.out.println("struct: " +
                               transferdata.return_struct().forename
                               + " " + transferdata.return_struct().surname
                               + " "
                               + transferdata.return_struct().phone);
            System.out.println("union: " +
                               transferdata.return_union().large());
            System.out.println("sequence: " +
                               transferdata.return_sequence()[3] +
                               ", length of sequence: " +
                               transferdata.return_sequence().length);
            System.out.println("array: " + transferdata.return_array()[1][2]);
            
            System.out.println("------Pseudo-Objects------");
            System.out.println("any-string: "+ 
                               transferdata.return_tk_wstring().
                               extract_string());
            System.out.println("any-struct: " + 
                               StructtypeHelper.
                               extract(transferdata.return_tk_struct()).
                               forename + " " + StructtypeHelper.extract
                               (transferdata.return_tk_struct()).
                               surname + " " +
                               StructtypeHelper.
                               extract(transferdata.return_tk_struct()).phone);
            System.out.println("any-union: " + UniontypeHelper.
                               extract
                               (transferdata.return_tk_union()).large());
            System.out.println("any-enum: " +
                               ColourHelper.extract
                               (transferdata.return_tk_enum()).
                               value());
            System.out.println("any-sequence: " + StrSeqHelper.
                               extract(transferdata.return_tk_sequence())[3]);
            System.out.println("any-array: " + LongArrayHelper.extract
                               (transferdata.return_tk_array())[1][2]);
        }
        
        return 0;
    }
}
