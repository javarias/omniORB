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
 *  This Java file implements the CDR encoder for use with the GIOP
 *  test adapter.
 *
 *****************************************************************************/

package giop_ta;

import com.testingtech.ttcn.tci.Type;
import com.testingtech.ttcn.tci.Value;
import com.testingtech.ttcn.tci.CharstringValue;
import de.tu_berlin.cs.uebb.muttcn.runtime.Data;
import com.testingtech.ttcn.tci.Encoder;
import de.tu_berlin.cs.uebb.ttcn.runtime.ForeignMessage ;
import com.testingtech.ttcn.tci.Value;
import com.testingtech.ttcn.tci.CharstringValue;
import com.testingtech.ttcn.tci.OctetstringValue;
import com.testingtech.ttcn.tci.RecordValue;
import com.testingtech.ttcn.tci.BooleanValue;
import com.testingtech.ttcn.tci.IntegerValue;
import com.testingtech.ttcn.tci.FloatValue;
import com.testingtech.ttcn.tci.UnionValue;
import com.testingtech.ttcn.tci.Type;
import com.testingtech.ttcn.tci.TciMessage ;
import com.testingtech.ttcn.tci.TciException ;

/**
 * This class implements the CDR Encoding.
 */
public class CDR_Encoder implements Encoder
{
    
    /**
     * Nothing to be done in the constructor.
     */
    public CDR_Encoder() {
    }

    byte[] mybytes = null;
    int offset = 0;

    /**
     * 'encode' encodes a GIOP message.
     *
     * @param value the TTCN-3 value of the message to encode
     * @return the TciMessage to send
     * @exception TciException if value is not valid
     */
    public TciMessage encode(Value value) throws TciException
    {
 	if(!(value instanceof Data)) 
        {
	    throw new TciException
                ("ValueInterface Encoding not yet implemented!");
        }
        offset = 0;
        messageLength(value);
        GIOP_TA.debug("Offset: " + offset);
        mybytes = new byte [offset];
        offset = 0;
        traverse(value);
        setMessageSize(offset);
        GIOP_TA.hexDump(mybytes, offset, "result:");

        return (new ForeignMessage(mybytes));
    }

    /**
     * 'setMessageSize' sets the correct message size in the GIOP message.
     *
     * @param len the size of the GIOP message
     */
    private void setMessageSize(int len)
    {
        len -= 12;
        mybytes [8] = (byte) (len >> 24);
        mybytes [9] = (byte) ((len >> 16) % 0x100);
        mybytes[10] = (byte) ((len >> 8) % 0100);
        mybytes[11] = (byte) (len % 0x100);
    }

    /**
     * 'traverse' traverses the TTCN-3 value and does most of the encoding.
     *
     * @param value the TTCN-3 value of the message to encode
     */
    private void traverse(Value value)
    {
        if (value.isOmit())
        {
            return;
        }
        switch (value.getType().getTypeClass())
        {
            case Type.CHARSTRING:
                GIOP_TA.debug("Charstring: " + value + " offset: " + offset);
                int len = ((CharstringValue) value).length ();
                if (offset > 11)
                {
                    offset += pad4 (offset);
                    mybytes [offset++] = (byte) (len+1 >> 24);
                    mybytes [offset++] = (byte) ((len+1 >> 16) % 0x100);
                    mybytes [offset++] = (byte) ((len+1 >> 8) % 0x100);
                    mybytes [offset++] = (byte) (len+1 % 0x100);
                }
                for (int i = 0; i < len; i++)
                {
                    mybytes [offset++] =
                        (byte) (((CharstringValue) value).getChar (i));
                }
                if (offset > 11)
                {
                    offset++;
                }
                break;
            case Type.INTEGER:
                if (value.getType().getName() == "CORBA_Short")
                {
                    GIOP_TA.debug("CORBA_Short");
                }
                GIOP_TA.debug("Integer: " +
                              ((IntegerValue) value).intValue());
                offset += pad4(offset);
                int myint = ((IntegerValue) value).intValue();
                mybytes [offset++] = (byte) (myint >> 24);
                mybytes [offset++] = (byte) ((myint >> 16) % 0x100);
                mybytes [offset++] = (byte) ((myint >> 8) % 0x100);
                mybytes [offset++] = (byte) (myint % 0x100);
                break;
            case Type.OCTETSTRING:
                GIOP_TA.debug("Octetstring: " + value);
                for (int i = 0; i < ((OctetstringValue) value).length(); i++)
                {
                    mybytes [offset++] =
                        (byte) (((OctetstringValue) value).getOctet(i));
                }
                break;
            case Type.RECORD:
                for (int i = 0;
                     i < ((RecordValue) value).getFieldNames().length;
                     i++)
                {
                    GIOP_TA.debug
                        ("Record field: " +
                         ((RecordValue) value).getFieldNames()[i]);
                    traverse (((RecordValue) value).getField
                              (((RecordValue) value).getFieldNames()[i]));
                }
                break;
            case Type.UNION:
                GIOP_TA.debug
                    ("Union field: " +
                     ((UnionValue) value).getPresentFieldName());
                traverse (((UnionValue) value).getField
                          (((UnionValue) value).getPresentFieldName()));
                break;
            default:
                GIOP_TA.warning("Unknown Type!");
        }
    }

    /**
     * 'pad4' returns the number of padding bytes for four byte padding.
     *
     * @param offset the current offset
     * @return the number of padding bytes
     */
    private int pad4(int offset)
    {
        return (4 - offset % 4) % 4;
    }

    /**
     * 'messageLength' calculates the length of the GIOP message to encode.
     *
     * @param value the TTCN-3 value of the message to encode
     */
    private void messageLength(Value value)
    {
        if (value.isOmit ())
        {
            return;
        }
        switch (value.getType ().getTypeClass ())
        {
            case Type.CHARSTRING:
                if (offset > 11)
                {
                    offset += pad4(offset);
                    offset += 5;
                }
                offset += ((CharstringValue) value).length ();
                return;
            case Type.INTEGER:
                offset += pad4(offset);
                offset += 4;
                return;
            case Type.OCTETSTRING:
                offset += ((OctetstringValue) value).length ();
                return;
            case Type.RECORD:
                for (int i = 0;
                     i < ((RecordValue) value).getFieldNames ().length;
                     i++)
                {
                    messageLength (((RecordValue) value).getField
                               (((RecordValue) value).getFieldNames ()[i]));
                }
                return;
            case Type.UNION:
                messageLength (((UnionValue) value).getField
                           (((UnionValue) value).getPresentFieldName ()));
                return;
            default:
                GIOP_TA.warning("Unknown Type!");
                return;
        }
    }
}
