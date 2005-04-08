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
 *  This Java file implements the CDR decoder for use with the GIOP
 *  test adapter.
 *
 *****************************************************************************/

package giop_ta;

import java.util.Arrays;
import com.testingtech.ttcn.tci.*;
import com.testingtech.ttcn.tri.*;
import de.tu_berlin.cs.uebb.ttcn.runtime.*;
import giop_ta.GIOP_TA;

/**
 * This class implements the CDR Decoding.
 */
public class CDR_Decoder implements Decoder 
{
    private static final TciValue typeServer =
        RB.runtimeBehavior.getTciValue();
    private static final TciManagement management =
        RB.runtimeBehavior.getTciManagement();
    private static final Type charstringType = typeServer.getCharstring();    
    private static final Type booleanType = typeServer.getBoolean();
    private static final Type integerType = typeServer.getInteger();
    private static final Type floatType = typeServer.getFloat();
    private static final Type octetstringType = typeServer.getOctetstring();
    private static Type GIOP_Reply_Type;
    private static Type GIOP_Header_Type;
    private static Type GIOP_Reply_Header_Type;
    private static Type GIOP_Reply_Body;
    private static Type GIOP_MessageError_Type;
    private static Type GIOP_LocateReply_Type;
    private static Type LocateReply_Header_Type;
    private static Module mod = management.getModule();

    /**
     * The constructor is mostly empty.
     */
    public CDR_Decoder() 
    {
        super();
    }

    /**
     * 'typeClass' returns a string representing of a TTCN-3 type.
     *
     * @param tc an integer value representing the TTCN-3 type
     * @return the name of the type
     */
    public String typeClass(int tc)
    {
        switch (tc)
        {
            case Type.INTEGER:              return "INTEGER";
            case Type.FLOAT:                return "FLOAT";
            case Type.BITSTRING:            return "BITSTRING";
            case Type.HEXSTRING:            return "HEXSTRING";
            case Type.OCTETSTRING:          return "OCTETSTRING";
            case Type.CHARSTRING:           return "CHARSTRING";
            case Type.UNIVERSAL_CHARSTRING: return "UNIVERSAL_CHARSTRING";
            case Type.RECORD:               return "RECORD";
            case Type.RECORD_OF:            return "RECORD_OF";
            case Type.ENUMERATED:           return "ENUMERATED";
            case Type.UNION:                return "UNION";
            case Type.BOOLEAN:              return "BOOLEAN";
            case Type.VERDICT:              return "VERDICT";
            case Type.COMPONENT:            return "COMPONENT";
            default:                        return "Unknown";
        }
    }

    /**
     * 'byte_arr_to_int' converts four bytes of a byte array into an integer.
     *
     * @param input byte array to convert
     * @param o offset in the byte array
     * @param le true, if little endian
     * @return the integer value
     */
    public static int byte_arr_to_int(byte[] input,
                                      int o,
                                      boolean le)
    {
        return
            le?
            (((input[o]   < 0)? input[o]   + 256: input[o]) +
             ((input[o+1] < 0)? input[o+1] + 256: input[o+1]) * 0x100 +
             ((input[o+2] < 0)? input[o+2] + 256: input[o+2]) * 0x10000 +
             ((input[o+3] < 0)? input[o+3] + 256: input[o+3]) * 0x1000000)
            :
            (((input[o]   < 0)? input[o]   + 256: input[o])   * 0x1000000 +
             ((input[o+1] < 0)? input[o+1] + 256: input[o+1]) * 0x10000 +
             ((input[o+2] < 0)? input[o+2] + 256: input[o+2]) * 0x100 +
             ((input[o+3] < 0)? input[o+3] + 256: input[o+3]));
    }
    
    /**
     * 'byte_arr_to_long' converts eight bytes of a byte array into a long.
     *  
     * @param input byte array to convert
     * @param o offset in the byte array
     * @param le true, if little endian
     * @return the long value 
     */
    public long byte_arr_to_long(byte[] input,
                                 int o,
                                 boolean le)
    {
        return
            le?
            (((input[o]   < 0)?  input[o]   + 256: input[o]) +
             ((input[o+1] < 0)? input[o+1] + 256: input[o+1]) * 0x100 +
             ((input[o+2] < 0)? input[o+2] + 256: input[o+2]) * 0x10000 +
             ((input[o+3] < 0)? input[o+3] + 256: input[o+3]) * 0x1000000 +
             ((input[o+4] < 0)? input[o+4] + 256: input[o+4]) * 0x100000000l +
             ((input[o+5] < 0)? input[o+5] + 256: input[o+5]) * 0x10000000000l +
             ((input[o+6] < 0)? input[o+6] + 256: input[o+6]) * 0x1000000000000l +
             ((input[o+7] < 0)? input[o+7] + 256: input[o+7]) * 0x100000000000000l)
            :
            (((input[o]   < 0)? input[o]   + 256: input[o])   * 0x100000000000000l +
             ((input[o+1] < 0)? input[o+1] + 256: input[o+1]) * 0x1000000000000l +
             ((input[o+2] < 0)? input[o+2] + 256: input[o+2]) * 0x10000000000l +
             ((input[o+3] < 0)? input[o+3] + 256: input[o+3]) * 0x100000000l +
             ((input[o+4] < 0)? input[o+4] + 256: input[o+4]) * 0x1000000 +
             ((input[o+5] < 0)? input[o+5] + 256: input[o+5]) * 0x10000 +
             ((input[o+6] < 0)? input[o+6] + 256: input[o+6]) * 0x100 +
             ((input[o+7] < 0)? input[o+7] + 256: input[o+7]));
    }
    
    /**
     * 'byte_arr_to_short' converts two bytes of a byte array into an integer.
     *
     * @param input byte array to convert
     * @param o offset in the byte array
     * @param le true, if little endian
     * @return the integer value
     */
    public int byte_arr_to_short(byte[] input,
                                 int o,
                                 boolean le)
    {
        return
            le?
            (((input[o]   < 0)? input[o]   + 256: input[o]) +
             ((input[o+1] < 0)? input[o+1] + 256: input[o+1]) * 0x100)
            :
            (((input[o]   < 0)? input[o]   + 256: input[o])   * 0x100 +
             ((input[o+1] < 0)? input[o+1] + 256: input[o+1]));
    }

     /**
     * 'int_to_float' converts an integer into a float.
     *
     * @param value integer value to convert
     * @return the float value
     */
    public float int_to_float (int value)
    {
        return Float.intBitsToFloat(value);
    }
    
     /**
     * 'int_to_double' converts a long value into a double.
     *
     * @param value long value to convert
     * @return the float (double) value, if TTthree will be updated, the
     * return value will be substituted by a double value.
     */
    public float long_to_double (long value)
    { // TThree still uses only floats

        return (float)Double.longBitsToDouble(value);
    }

     /**
     * 'byte_arr_to_string' converts bytes of a byte array into a string.
     *
     * @param input byte array to convert
     * @param offset offset in the byte array
     * @return the string value
     */
    public String byte_arr_to_string (byte[] receive_mesg,
                                      int offset,
                                      boolean le)
    {

        return new String(receive_mesg, offset + 4,
                          byte_arr_to_int(receive_mesg, offset, le) - 1);
    }
    
    /**
     * 'decode' decodes a GIOP message.
     *
     * @param message a 'TciMessage' to decode
     * @param type expected TTCN-3 type of the message
     * @return the decoded message as TTCN-3 'Value'
     * @exception TciException not yet implemented
     */
    public Value decode(TciMessage message,
			Type type)
	throws TciException
    {
        int octets = message.length()/8;
        boolean le = false;
        GIOP_TA.debug("Defining module: " + type.getDefiningModule() +
                      " type: " + type.getName() +
                      " typeclass: " + typeClass(type.getTypeClass()) +
                      " msg-length: " + octets);
        GIOP_TA.hexDump(message.toByteArray(), octets, "msg to decode:");
        if (type.getTypeClass() == Type.RECORD)
        {
            try
            {
                if (octets < 12)
                {
                    GIOP_TA.warning("Message too short!");
                    return null;
                }

                GIOP_Header_Type = typeServer.forName(mod,
						      "GIOP_Header_Type");
                RecordValue header =(RecordValue)
                    GIOP_Header_Type.newInstance();
                CharstringValue magic =
                    (CharstringValue) charstringType.newInstance();

                IntegerValue intVal = null;

                int messagetype = -1;

                byte[] receive_mesg = message.toByteArray();
                String magicString = "";

                for (int i=0; i < 4; i++)
                {
                    magicString = magicString +
                        String.valueOf((char)receive_mesg[i]);
                }

                magic.setCharstring(magicString);
                header.setField("magic", magic);

                OctetstringValue octstrval =
                    (OctetstringValue) octetstringType.newInstance();
                byte[] versionString = {receive_mesg[4], receive_mesg[5]};
                octstrval.setOctetstring(versionString);
                header.setField("version", octstrval);

                byte[] byteorder = {receive_mesg[6]};
                octstrval.setOctetstring(byteorder);
                header.setField("byteorder", octstrval);
                le = (byteorder[0] == 1);

                byte[] messagetypeOctet = {receive_mesg[7]};
                octstrval.setOctetstring(messagetypeOctet);
                header.setField("messagetype", octstrval);
                messagetype = (int) messagetypeOctet[0];

                intVal = (IntegerValue) integerType.newInstance();
                intVal.setInt(byte_arr_to_int(receive_mesg, 8, le));
                header.setField("messagesize", intVal);
                switch (messagetype)
                {
                    case 1:     // reply
                    {
                        if (octets < 24)
                        {
                            GIOP_TA.warning("Reply too short!");
                            return null;
                        }
                        Value reply_body = null;
                        GIOP_Reply_Header_Type =
                            typeServer.forName(mod, "Reply_Header_Type");
                        RecordValue reply_header =
                            (RecordValue)
                            GIOP_Reply_Header_Type.newInstance();

                        intVal.setInt(byte_arr_to_int(receive_mesg, 12, le));
                        reply_header.setField("request_id", intVal);
                        intVal.setInt(byte_arr_to_int(receive_mesg, 16, le));
                        reply_header.setField("reply_status", intVal);
                        intVal.setInt(byte_arr_to_int(receive_mesg, 20, le));
                        reply_header.setField("service_context", intVal);

                        if (type.getName() == "GIOP_Reply_Boolean_Type")
                        {
                            GIOP_TA.debug("boolean");
                            GIOP_Reply_Body = typeServer.forName
                                (mod, "Reply_Body_Boolean_Type");
                            reply_body =
                                (OctetstringValue)
                                octetstringType.newInstance();
                            byte[] ret_value = {receive_mesg[24]};
                            ((OctetstringValue)
                             reply_body).setOctetstring(ret_value);
                            GIOP_Reply_Type = typeServer.forName
                                    (mod, "GIOP_Reply_Boolean_Type");
                        }
                        else if (type.getName() == "GIOP_Reply_Long_Type")
                        {
                            GIOP_TA.debug("long");
                            GIOP_Reply_Body = typeServer.forName
                                (mod, "Reply_Body_Long_Type");
                            reply_body = (IntegerValue)
                                integerType.newInstance();
                            ((IntegerValue) reply_body).setInt
                                (byte_arr_to_int(receive_mesg, 24, le));
                            GIOP_Reply_Type = typeServer.forName
                                (mod, "GIOP_Reply_Long_Type");
                        }
                        else if (type.getName() == "GIOP_Reply_LongLong_Type")
                        {
                            GIOP_TA.debug("longlong");
                            GIOP_Reply_Body = typeServer.forName
                                (mod, "Reply_Body_LongLong_Type");
                            reply_body = (IntegerValue)
                                integerType.newInstance();
                            ((IntegerValue) reply_body).setInt
                                // if TTthree will be updated, long
                                // will be used instead of cast to
                                // integer
                                ((int) byte_arr_to_long(receive_mesg, 24, le));
                            GIOP_Reply_Type = typeServer.forName
                                (mod, "GIOP_Reply_LongLong_Type");
                        }
                        else if (type.getName() == "GIOP_Reply_Short_Type")
                        {
                            GIOP_TA.debug("short");
                            GIOP_Reply_Body = typeServer.forName
                                (mod, "Reply_Body_Short_Type");
                            reply_body = (IntegerValue)
                                integerType.newInstance();
                            ((IntegerValue) reply_body).setInt
                                (byte_arr_to_short(receive_mesg, 24, le));
                            GIOP_Reply_Type = typeServer.forName
                                (mod, "GIOP_Reply_Short_Type");
                        }
                        else if (type.getName() == "GIOP_Reply_Octet_Type")
                        {
                            GIOP_TA.debug("octet");
                            GIOP_Reply_Body = typeServer.forName
                                (mod, "Reply_Body_Octet_Type");
                            reply_body =
                                (OctetstringValue)
                                octetstringType.newInstance();
                            byte[] ret_value = {receive_mesg[24]};
                            ((OctetstringValue)
                             reply_body).setOctetstring(ret_value);
                            GIOP_Reply_Type = typeServer.forName
                                (mod, "GIOP_Reply_Octet_Type");
                        }
                        else if (type.getName() == "GIOP_Reply_Float_Type")
                        {
                            GIOP_TA.debug("float");
                            GIOP_Reply_Body = typeServer.forName
                                (mod, "Reply_Body_Float_Type");
                            reply_body = (FloatValue)
                                floatType.newInstance();
                            ((FloatValue) reply_body).setFloat
                                (int_to_float(byte_arr_to_int
                                              (receive_mesg, 24, le)));
                            GIOP_Reply_Type = typeServer.forName
                                (mod, "GIOP_Reply_Float_Type");
                        }
                        else if (type.getName() == "GIOP_Reply_Double_Type")
                        {
                            GIOP_TA.debug("double");
                            GIOP_Reply_Body = typeServer.forName
                                (mod, "Reply_Body_Double_Type");
                            reply_body = (FloatValue)
                                floatType.newInstance();
                            ((FloatValue) reply_body).setFloat
                                (long_to_double(byte_arr_to_long
                                                (receive_mesg, 24, le)));
                            GIOP_Reply_Type = typeServer.forName
                                (mod, "GIOP_Reply_Double_Type");
                        }
                        else if (type.getName() == "GIOP_Reply_Char_Type")
                        {
                            GIOP_TA.debug("char");
                            GIOP_Reply_Body = typeServer.forName
                                (mod, "Reply_Body_Char_Type");
                            reply_body =
                                (OctetstringValue)
                                octetstringType.newInstance();
                            byte[] ret_value = {receive_mesg[24]};
                            ((OctetstringValue)
                             reply_body).setOctetstring(ret_value);
                            GIOP_Reply_Type = typeServer.forName
                                (mod, "GIOP_Reply_Char_Type");
                        }
                        else if (type.getName() == "GIOP_Reply_String_Type")
                        {
                            GIOP_TA.debug("string");
                            GIOP_Reply_Body = typeServer.forName
                                (mod, "Reply_Body_String_Type");
                            reply_body =
                                (CharstringValue)
                                charstringType.newInstance();
                            ((CharstringValue)
                             reply_body).setCharstring
                                (byte_arr_to_string(receive_mesg, 24, le));
                            GIOP_Reply_Type = typeServer.forName
                                (mod, "GIOP_Reply_String_Type");
                        }
                        else if (type.getName() == "GIOP_Reply_Enum_Type")
                        {
                            GIOP_TA.debug("enum");
                            GIOP_Reply_Body = typeServer.forName
                                (mod, "Reply_Body_Enum_Type");
                            reply_body = (IntegerValue)
                                integerType.newInstance();
                            ((IntegerValue) reply_body).setInt
                                (byte_arr_to_int(receive_mesg, 24, le));
                            GIOP_Reply_Type = typeServer.forName
                                (mod, "GIOP_Reply_Enum_Type");
                        }
                        else
                        {
                            GIOP_TA.warning("Decoding of type "
                                            + type.getName() +
                                            " not yet implemented!");
                            return null;
                        }
                        RecordValue reply =
                            (RecordValue) GIOP_Reply_Type.newInstance();
                        reply.setField("giop_header", header);
                        reply.setField("reply_header", reply_header);
                        reply.setField("reply_body", reply_body);
                        return reply;
                    }
                    case 4:     // locate reply
                    {
                        GIOP_TA.debug("locate reply");
                        if (octets < 20)
                        {
                            GIOP_TA.warning("Reply too short!");
                            return null;
                        }
                        if (type.getName() == "GIOP_LocateReply_Type")
                        {
                            GIOP_LocateReply_Type =
                                typeServer.forName(mod,
						   "GIOP_LocateReply_Type");
                            RecordValue loc_reply =
                                (RecordValue)
                                GIOP_LocateReply_Type.newInstance();
                            LocateReply_Header_Type =
                                typeServer.forName(mod,
						   "LocateReply_Header_Type");
                            RecordValue loc_rep_header =
                                (RecordValue)
                                LocateReply_Header_Type.newInstance();
                            intVal.setInt(byte_arr_to_int(receive_mesg,
                                                          12, le));
                            loc_rep_header.setField("request_id", intVal);
                            intVal.setInt(byte_arr_to_int(receive_mesg,
                                                          16, le));
                            loc_rep_header.setField("locate_status", intVal);
                            loc_reply.setField("giop_header", header);
                            loc_reply.setField("locateReply_header",
					       loc_rep_header);
                            return loc_reply;
                        }
                        else
                        {
                            GIOP_TA.debug("Decoding of type "
                                          + type.getName() +
                                          " not yet implemented!");
                            return null;
                        }
                    }
                    case 6:     // error message
                    {
                        GIOP_TA.debug("msgerror");
                        if (type.getName() == "GIOP_MessageError_Type")
                        {
                            GIOP_MessageError_Type =
                                typeServer.forName(mod,
						   "GIOP_MessageError_Type");
                            RecordValue error =
                            (RecordValue) GIOP_MessageError_Type.newInstance();
                            error.setField("giop_header", header);
                            return error;
                        }
                        else
                        {
                            GIOP_TA.warning("Decoding of type "
                                            + type.getName() +
                                            " not yet implemented!");
                            return null;
                        }
                    }
                    default:    // else
                    {
                        GIOP_TA.warning("Decoding of messagetype "
                                        + messagetype
                                        + " not yet implemented!");
                        return null;
                    }
                }
            }
            catch (Exception e)
            {
                GIOP_TA.warning("Exception in Decoder:" + e);
            }
        }
        GIOP_TA.debug("end of decode");
        return null;
    }
}
