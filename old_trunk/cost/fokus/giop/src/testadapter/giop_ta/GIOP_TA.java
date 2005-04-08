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
 *  This is the test adapter for the GIOP executable test suite.
 *
 *****************************************************************************/

package giop_ta;

import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;

import java.io.BufferedOutputStream;
import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.IOException;

import java.util.Arrays;

import com.testingtech.ttcn.tri.*;
import com.testingtech.ttcn.tci.*;
import de.tu_berlin.cs.uebb.ttcn.runtime.RB;

import org.etsi.ttcn.tri.*;

/**
 * This class implements a test adapter for TTCN-3 based conformance
 * tests of GIOP/IIOP implementations.
 */
public class GIOP_TA extends TestAdapter
{
    boolean thread_running = true;
    Socket socket = null;
    Thread mythread = null;
    byte[] magic = {0x47, 0x49, 0x4f, 0x50}; // "GIOP"
    byte[] byte_buf = null;

    private static final TciValue typeServer =
        RB.runtimeBehavior.getTciValue();
    private static final Type octetstringType = typeServer.getOctetstring();
    private static final Type charstringType = typeServer.getCharstring();
    private static final TciManagement management =
        RB.runtimeBehavior.getTciManagement();
    private static Module mod = management.getModule();
    private int port = 0;
    private String server = "";
    BufferedOutputStream output;
    BufferedInputStream input;
    private static int debugLevel = 0; //set to 0, 1, or 2

    /**
     * The constructor calls the IOR analyser and sets the server IP
     * address and TCP port.
     */
    public GIOP_TA()
    {
        super();
        IOR.analyse_ior();
        port = IOR.get_port();
        server = IOR.get_host();
        out = System.out ;
	err = System.err ;
    }

    /**
     * 'triSend' sends out the encoded GIOP messages via a socket.
     *
     * @param componentId id of the component calling triSend
     * @param tsiPortId id of the port to be used
     * @param address not used
     * @param sendMessage TriMessage to be sent
     * @return the status
     */
    public TriStatus triSend(TriComponentId componentId,
                             TriPortId tsiPortId,
                             TriAddress address,
                             TriMessage sendMessage)
    {
        debug("In triSend");
        debug("  componentId: " + componentId);
	debug("  tsiPortId: " + tsiPortId);
        if (tsiPortId.getPortName().equals("giop_port"))
        {
            byte[] mesg = sendMessage.getEncodedMessage();

            debug(mesg.length + " Message will be sent");
            hexDump (mesg, mesg.length, "before sending:");

            try
            {
                output = new BufferedOutputStream(socket.getOutputStream());
                output.write(mesg, 0, mesg.length);
                output.flush();
            }
            catch (IOException e)
            {
                warning("write" + e);
                thread_running = false;
            }
        }
	return new TriStatusImpl();
    }

    /**
     * 'byteToHex' converts a byte to its hexadecimal string representation.
     *
     * @param b the byte to convert
     * @return a two-character string
     */
    public static String byteToHex(byte b)
    {
        int i = (b >= 0) ? b : (b + 256);
        return (i < 16 ? "0" : "") + Integer.toHexString(i);
    }

    /**
     * 'hexDump' prints a byte array in a hexadecimal representation.
     * Sixteen bytes are printed per line.
     *
     * @param bytes the byte array to print
     * @param len the length of the byte array
     * @param title a title line for the hex dump
     */
    public static void hexDump(byte[] bytes,
			       int len,
			       String title)
    {
        if (debugLevel < 2)
        {
            return;
        }
        System.out.println(title);
        for (int i = 0; i < len; i++)
        {
            if (i % 16 == 0 && i != 0)
            {
                System.out.println();
            }
            System.out.print(byteToHex(bytes[i])+ " ");
        }
        System.out.println();
    }

    /**
     * 'fatal' prints out an error message.
     *
     * @param output the error text
     */
    public static void fatal(String output)
    {
        System.out.println("Fatal: " + output);
    }

    /**
     * 'warning' prints out a warning message.
     *
     * @param output the warning text
     */
    public static void warning(String output)
    {
        System.out.println("Warning: " + output);
    }

    /**
     * 'info' prints out an informational message.
     *
     * @param output the information text
     */
    public static void info(String output)
    {
        if (debugLevel >= 1)
        {
            System.out.println("Info: " + output);
        }
    }

    /**
     * 'debug' prints out a debug message.
     *
     * @param output the debug text
     */
    public static void debug(String output)
    {
        if (debugLevel >= 2)
        {
            System.out.println("Debug: " + output);
        }
    }

    public TriStatus triExecuteTestcase(TriTestCaseId testcase,
                                        TriPortIdList tsiList)
    {
	return new TriStatusImpl();
    }

    /**
     * 'triMap' opens and maintains the GIOP connection.
     *
     * @param compPortId id of the test component port
     * @param tsiPortId id of the test system interface port
     * @return the status
     */
    public TriStatus triMap(TriPortId compPortId,
			    TriPortId tsiPortId)
    {
        TriStatus mapStatus = CsaDef.triMap(compPortId, tsiPortId);
	if (mapStatus.getStatus() != TriStatus.TRI_OK)
	{
	    return mapStatus ;
	}
	if (tsiPortId.getPortName().equals("giop_port"))
	{
            debug("comp: " + compPortId.getComponent());
            debug("tsiPort: " + tsiPortId);

            final TriComponentId cid  = compPortId.getComponent();
            final TriPortId pid  = tsiPortId;
            debug("triMap");
            try
            {
                socket = new Socket(server, port);
                socket.setSoTimeout(0);
                info("Connected with server " +
                     socket.getInetAddress() +
                     ":" + socket.getPort());
            }
            catch (UnknownHostException e)
            {
                warning("getInetAddress(): " + e);
                System.exit(-1);
            }
            catch (IOException e)
            {
                warning("new socket: " + e);
                System.exit(-1);
            }
            new Thread() {
                /**
		 * 'run' reads the data from the GIOP socket.  It
		 * copies the GIOP messages into newly created
		 * TriMessages, that are queued to the TTCN-3
		 * executable.
		 */
		public void run()
                {
                    debug("thread!");
                    try
                    {
                        thread_running = true;
                        byte_buf = new byte[12];
                        input = new BufferedInputStream
                            (socket.getInputStream());
                        while (thread_running == true)
                        {
                            try
                            {
                                int in = 0;
                                try
                                {
                                    in = input.read(byte_buf, 0, 12);
                                    hexDump (byte_buf, in,
                                             "read bytes (good):");
                                }
                                catch (Exception e)
                                {
                                    hexDump (byte_buf, in, "read bytes:");
                                    debug("Exception " + e);
                                    if (input.available() > 0)
                                    {
                                        continue;
                                    }
                                    else
                                    {
                                        thread_running = false;
                                    }
                                }
                                if (-1 == in)
                                {
                                    info("socket closed by server");
                                    thread_running = false;
                                }
                                else
                                {
                                    debug("Got " + in + " bytes from server");
                                    int len = byte_buf[8] * 0x1000000
                                        + byte_buf[9] * 0x10000
                                        + byte_buf[10] * 0x100
                                        + byte_buf[11];
                                    debug("GIOP message length: " + len);
                                    byte[] real_byte_buf = new byte[len + 12];
                                    for (int i=0; i < 12; i++)
                                    {
                                        real_byte_buf[i] = byte_buf[i];
                                    }
                                    try
                                    {
                                        in = input.read
                                            (real_byte_buf, 12, len);
                                    }
                                    catch (Exception e)
                                    {
                                        warning("read(): " + e);
                                    }
                                    if (-1 == in) 
                                    {
                                        info("socket closed by server");
                                        thread_running = false;
                                    }
                                    else
                                    {
                                        TriMessage trimessage =
                                            new TriMessageImpl(real_byte_buf);
                                        hexDump (real_byte_buf, len+12,
                                                 "real_byte_buf");
                                        Cte.triEnqueueMsg
                                            (pid,
                                             new TriAddressImpl
                                                 (new byte[] {}),
                                             cid,
                                             trimessage);
                                    }
                                    debug("After reading");
                                }
                            }
                            catch (Exception e)
                            {
                                warning("read: " + e);
                                thread_running = false;
                            }
                        }
                    }
                    catch (IOException e)
                    {
                        warning("new, write, read" + e);
                    }
                }
            }.start();
            debug("nach thread!");
	}
	else if (tsiPortId.getPortName().equals("MyPortAsync"))
	{
	    return new TriStatusImpl();
	}
	else
	{
	    return new TriStatusImpl("triMap: (from: " + compPortId +
                                     ", to: " + tsiPortId +
                                     ") not implemented");
	}

	return new TriStatusImpl();
    }

    /**
     * 'triExternalFunction' calls TTCN-3 external functions.
     *
     * @param functionId the id of the external function
     * @param parameterList the parameter list for the external function
     * @param returnValue the TTCN-3 return value of the external function
     * @return the status
     */
    public TriStatus triExternalFunction(TriFunctionId functionId,
					 TriParameterList parameterList,
					 TriParameter returnValue)
    {
        enterTriExternalFunction();
        if (functionId.getFunctionName().equals("get_object_key"))
        {
            try
            {
                OctetstringValue theReturnValue = (OctetstringValue)
                    octetstringType.newInstance();
                theReturnValue.setOctetstring(IOR.get_object_key());

                TriStatus returnState = setValue(theReturnValue, returnValue);
                leaveTriExternalFunction();
                return returnState;
            }
            catch (TciException tix)
            {
                return new TriStatusImpl("Value error");
            }
        }
        leaveTriExternalFunction();
        return new TriStatusImpl();
    }

    /**
     * 'triUnmap' stops the receive thread and closes the TCP socket.
     */
    public void triUnmap()
    {
        debug("end of test");
        thread_running = false;
        if (socket != null)
        {
            try
            {
                socket.close();
                info("Socket closed by client");
            }
            catch (IOException e)
            {
                warning("close" + e);
            }
        }
        debug("thread_running = false");
        debug("Cancel");
    }

    /**
     * 'getEncoder' creates and returns a CDR encoder object.
     *
     * @param encodingName the name of the encoding
     * @return the encoder object
     * @exception TciException if the encoding is unknown
     */
    public Encoder getEncoder(String encodingName) throws TciException
    {
	Encoder encoder = (Encoder) encoders.get(encodingName);
	if (encoder != null)
	{
	    return encoder;
	}
	encoder = new CDR_Encoder();
	if (encoder != null)
	{
	    encoders.put(encodingName, encoder);
	}
	else
	{
	    throw new TciException("Unknown encoding " + encodingName);
	}
	return encoder;
    }

    /**
     * 'getDecoder' creates and returns a CDR decoder object.
     *
     * @param decodingName the name of the decoding
     * @return the decoder object
     * @exception TciException if the decoding is unknown
     */
    public Decoder getDecoder(String decodingName) throws TciException
    {
        Decoder decoder = (Decoder) decoders.get(decodingName);
        if (decoder != null)
	{
            return decoder;
	}
        decoder = super.getDecoder(decodingName);
        if (decoder != null)
	{
	    return decoder;
	}
        if (decodingName.equals(""))
        {
            decoder = new CDR_Decoder();
            decoders.put(decodingName, decoder);
        }
        else
        {
            throw new TciException("Unknown decoding " + decodingName);
        }
        return decoder;
    }
}

/**
 * The IOR class is for analysing the IOR file.  It contains important
 * IOR information like the object key, the IP number and the TCP
 * port.
 */
class IOR
{
    public static int object_key_length = 0;
    private static String object_key 	= "0";
    private static int port 		= 0;
    private static String server 	= "0";
    public static byte[] buff_objkey 	= null;

    public static void remove_padding(long[] byte_position,
                                      int alignment,
                                      FileInputStream file) throws IOException
    {
        while (byte_position[0] % alignment != 0)
        {
            byte_position[0]++;
            try
            {
                file.skip(2);
            }
            catch (java.io.IOException e)
            {
                throw e;
            }
        }
    }

    /**
     * 'analyse_ior' opens, parses, and analyses the IOR file.
     */
    public static void analyse_ior()
    {
        String filename          = "TransferData.ref";
        int length               = 0;
        long[] byte_position     = {0}; //starts after IOR: in IOR file
        String str_value         = "0"; // to get some bytes into one string
        int numeric_length       = 0;
        int profile_length       = 0;
        int host_length          = 0;
        boolean le_flag          = false;
        byte[] ior               = {'I', 'O', 'R', ':'};
        byte[] big_endian        = {'0', '0'};
        byte[] little_endian     = {'0', '1'};
        byte[] tagged_profile    = {'0','0','0','0','0','0','0','1'};
        byte[] tagged_profile_le = {'0','1','0','0','0','0','0','0'};
        byte[] tag_internet_iop  = {'0','0','0','0','0','0','0','0'};
        byte[] iiop_version      = {'0','1','0','2'};
        byte[] iiop_version_le   = {'0','2','0','1'};
        byte[] buff_2            = new byte[2];
        byte[] buff_4            = new byte[4];
        byte[] buff_8            = new byte[8];
        FileInputStream file     = null;
        Socket socket            = null;

        try
        {
            file = new FileInputStream(filename);
        }
        catch(java.io.FileNotFoundException e)
        {
            GIOP_TA.warning("no IOR file");
            System.exit(1);
        }
        try
        {
            file.read(buff_4, 0, 4); // ior
            if (Arrays.equals(buff_4, ior))
            {
                GIOP_TA.debug("IOR file");
            }
            else
            {
                throw new Exception("No IOR file");
            }
            length = file.read(buff_2, 0, 2); // byte order
            if (Arrays.equals(buff_2, big_endian))
            {
                GIOP_TA.debug("big-endian");
            }
            else if (Arrays.equals(buff_2, little_endian))
            {
                le_flag = true;
            }
            else
            {
                throw new Exception("something wrong with endianess");
            }
            byte_position[0] = length/2;
            GIOP_TA.debug("length:" + length);
            byte_position[0] += 3; // padding
            file.skip(3 * 2); // the double of increment of byteposition
            file.read(buff_8, 0, 8); // length of type id
            numeric_length = byte_arr_to_int(hex_to_byte(buff_8), 0, le_flag);
            file.skip(numeric_length * 2); // go to next
            byte_position[0] += numeric_length;
            remove_padding(byte_position, 4, file);
            file.read(buff_8, 0, 8); // Tagged Profile: unsigned long + octets
            if (Arrays.equals(buff_8, le_flag? tagged_profile_le: tagged_profile))
            {
                GIOP_TA.debug("tagged_profile");
            }
            else
            {
                throw new Exception("no tagged_profile");
            }
            byte_position[0] += 4;

            file.read(buff_8, 0, 8);
            if (Arrays.equals(buff_8, tag_internet_iop))
            {
                GIOP_TA.debug("tag_internet_iop");
            }
            else
            {
                throw new Exception("no tag_internet_iop");
            }
            byte_position[0] += 4;
            file.read(buff_8, 0, 8);
            profile_length = byte_arr_to_int(hex_to_byte(buff_8), 0, le_flag);
            GIOP_TA.debug("profile_length: " + profile_length);
            if (profile_length > 0)
            {
                file.read(buff_2, 0, 2);
                byte_position[0] += 4;
                GIOP_TA.debug("byte position: " + byte_position[0]);
                if (Arrays.equals(buff_2, big_endian))
                {
                    GIOP_TA.debug("big endian");
                    byte_position[0] += 1;
                    file.read(buff_4, 0, 4);
                    if (Arrays.equals(buff_4,
                                      le_flag? iiop_version_le: iiop_version))
                    {
                        GIOP_TA.debug("IIOP version 1.2");
                        byte_position[0] += 2;
                        remove_padding(byte_position, 4, file);
                        GIOP_TA.debug("byte position: " + byte_position[0]);
                        file.read(buff_8, 0, 8);
                        host_length = byte_arr_to_int(hex_to_byte(buff_8),
                                                      0, le_flag);
                        GIOP_TA.debug("host_length: " + host_length);
                        byte_position[0] += 4;
                        byte[] buff_host = new byte[host_length*2];
                        file.read(buff_host, 0, host_length*2);
                        server = new String(hex_to_byte(buff_host));
                        GIOP_TA.debug("host: " + server);
                        byte_position[0] += host_length;
                        remove_padding(byte_position, 2, file);
                        file.read(buff_4, 0, 4);
                        port = byte_arr_to_short(hex_to_byte(buff_4),
                                                 0, le_flag);
                        GIOP_TA.debug("port: " + port);
                        byte_position[0] += 2;
                        remove_padding(byte_position, 4, file);
                        file.read(buff_8, 0, 8);
                        object_key_length = byte_arr_to_int(hex_to_byte(buff_8),
                                                            0, le_flag);
                        GIOP_TA.debug("object_key_length: " +
                                      object_key_length);
                        byte_position[0] += 4;
                        buff_objkey = new byte[object_key_length*2];
                        file.read(buff_objkey, 0, buff_objkey.length);
                        GIOP_TA.hexDump(buff_objkey, buff_objkey.length,
                                        "object key (ASCII: ");
                        object_key = new String(buff_objkey);
                    }
                }
                else if (Arrays.equals(buff_2, little_endian))
                {
                    GIOP_TA.debug("little endian");
                }
                else
                {
                    throw new Exception("Neither big nor little endian");
                }
            }
            else
            {
                throw new Exception("No IOP");
            }
            file.close();
        }
        catch (java.io.IOException e)
        {
            GIOP_TA.warning("read or write error");
        }
        catch (Exception exc)
        {
            GIOP_TA.fatal(exc.toString());
            try
            {
                file.close();
                System.exit(1);
            }
            catch (java.io.IOException e)
            {
                GIOP_TA.warning("read or write error");
            }
        }
    }

    public static byte[] hex_to_byte(byte[] hex)
    {
        byte[] result = new byte[hex.length/2];
        for (int n = 0, i = 0; n < hex.length; n += 2, i++)
        {
            result[i] = (byte) ((Character.digit((char) hex[n], 16) << 4)
                        + (byte) (Character.digit((char) hex[n + 1], 16)));
        }
        return result;
    }

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

    public static int byte_arr_to_short(byte[] input,
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
     * 'get_object_key' returns the object key as in the IOR file.
     *
     * @return the object key
     */
    public static byte[] get_object_key()
    {
        if (object_key.length() < 10)
        {
            GIOP_TA.debug("length of object_key: " + object_key.length());
            return null;
        }
        byte[] obj_key = new byte[object_key.length()/2 + 4];

        obj_key[0] = (byte) (object_key_length >> 24);
        obj_key[1] = (byte) ((object_key_length >> 16) % 0x100);
        obj_key[2] = (byte) ((object_key_length >> 8) % 0x100);
        obj_key[3] = (byte) (object_key_length % 0x100);

        for(int n = 0, i = 4 ; i < obj_key.length; n += 2, i++)
        {
            obj_key[i] = (byte) ((Character.digit((char) buff_objkey[n],
                                                  16) << 4) + (byte)
                                 (Character.digit((char) buff_objkey[n + 1],
                                                  16)));
        }
        GIOP_TA.hexDump(obj_key, obj_key.length, "obj key");
        return obj_key;
    }

    /**
     * 'get_host' returns the IP number as in the IOR file.
     *
     * @return the IP number
     */
    public static String get_host()
    {
        GIOP_TA.debug("Server: " + server);
        return server;
    }

    /**
     * 'get_port' returns the TCP port number as in the IOR file.
     *
     * @return the TCP port number
     */
    public static int get_port()
    {
        GIOP_TA.debug("Port: " + port);
        return port;
    }
}
