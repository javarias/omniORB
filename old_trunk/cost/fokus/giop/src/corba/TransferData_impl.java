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
 *  IDL skeleton implementation for the server test component of the GIOP/IIOP
 *  test suite.
 *
 *****************************************************************************/

package transferdata;
import org.omg.CORBA.*;
import transferdata.TransferDataPackage.*;
import java.math.*;

/**
 *  This java class is the implementation of the IDL skeleton.
 */
public class TransferData_impl extends TransferDataPOA
{

    private ORB orb_;
    
    public TransferData_impl(ORB orb)  // Initialisation of orb
    {
        orb_ = orb;
    }

    public char return_char()
    {
        System.out.println("Server: return_char");
        char sign = '$';
        return sign;
    }

    public String return_string()
    {
        System.out.println("Server: return_string");
        String word = "hello!";
        return word;
    }

    public String return_wstring()
    {
        return return_string();
    }

    public byte return_octet()
    {
        System.out.println("Server: return_octet");
        byte oct = 8;
        return oct;
    }

    public short return_short()
    {
        System.out.println("Server: return_short");
        short num = 32767;
        return num;
    }

    public short return_ushort()
    {
        return return_short(); 
    }

    public int return_long()
    {
        System.out.println("Server: return_long");
        int num = 2147483647;
        return num;
    }

    public int return_ulong()
    {
        return return_long();
    }

    public long return_longlong()
    {
        System.out.println("Server: return_longlong");
        long num = 92233l; // numbers bigger than integer do not
                           // work with TTthree at the moment
        return num;
    }
    
    public long return_ulonglong()
    {
        System.out.println("Server: return_ulonglong");

        return return_longlong();   
    }

    public float return_float()
    {
        System.out.println("Server: return_float");
        float num = 3.4E38f;
        return num;
    }

    public Colour return_enum()
    {
        System.out.println("Server: return_enum");
        Colour e_type = Colour.red;
        return e_type;
    }

    public boolean return_boolean()
    {
        System.out.println("Server: return_boolean");
        boolean bool = true;
        return bool;
    } 

    public double return_double()
    {
        System.out.println("Server: return_double");
        //double num = 1.7E308d;
        double num = 3.14d;
        return num;
    }
    
    public BigDecimal return_fixed()
    {
        System.out.println("Server: return_BigDecimal");
        return BigDecimal.valueOf(20000);
    }
    
    public Structtype return_struct()
    {
        System.out.println("Server: return_struct");
        Structtype name = new Structtype("Anna", "Mueller", 7352121);
        
        return name;
    }

    public Uniontype return_union()
    {
        System.out.println("Server: return_union");
        Uniontype union = new Uniontype();
        byte b = 12;
        short s = 123;

        union.large(1444);
        return union;
    }

    public String[] return_sequence()
    {
        System.out.println("Server: return_sequence");
        String[] fruit = {"banana", "orange", "lemon", "apple", "strawberry"};
        return fruit;
    }

    public int[][] return_array()
    {
        System.out.println("Server: return_array");
        int[][] matrix = new int[3][3];
        for (int i = 0; i < matrix.length; i++)
	{
            for(int j = 0; j < matrix[i].length; j++)
            {
                matrix[i][j] = i + j;
                System.out.print(matrix[i][j] + "  ");
            }
            System.out.println();
        }
        System.out.println();
        return matrix;
    }
    
    //The Pseudo-Object Types
    public Any return_tk_wstring()
    {
        Any any = orb_.create_any();

        any.insert_string(return_wstring());

        return any;
    }

    public Any return_tk_struct()
    {
        Any any = orb_.create_any();
     
        StructtypeHelper.insert(any, return_struct());

        return any; 
    }

    public Any return_tk_union()
    {
        Any any = orb_.create_any();
     
        UniontypeHelper.insert(any, return_union());
        
        return any;
    }

    public Any return_tk_enum()
    {
        Any any = orb_.create_any();

        ColourHelper.insert(any, return_enum()); 

        return any;
    }

    public Any return_tk_sequence()
    {
        Any any = orb_.create_any();
        
        StrSeqHelper.insert(any, return_sequence());
        
        return any;
    }

    public Any return_tk_array()
    {
        Any any = orb_.create_any();
        
        LongArrayHelper.insert(any, return_array());
        return any;
    }
}


