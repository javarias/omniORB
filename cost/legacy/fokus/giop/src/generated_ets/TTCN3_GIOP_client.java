package generated_ttcn;
import java.util.Arrays;
import java.util.Collection;
import de.tu_berlin.cs.uebb.ttcn.runtime.*;
import de.tu_berlin.cs.uebb.ttcn.runtime.Void;
import de.tu_berlin.cs.uebb.muttcn.runtime.*;
import de.tu_berlin.cs.uebb.muttcn.runtime.Float;
import de.tu_berlin.cs.uebb.muttcn.runtime.Module;
import com.testingtech.ttcn.tci.Type;
import com.testingtech.ttcn.tci.Value;


public class TTCN3_GIOP_client extends Module {
  final TTCN3_GIOP_client TTCN3_GIOP_client = this;

  public static final String moduleName = "GIOP_client";

  protected String getModuleId() {
    return moduleName;
  }

  static final String sourceFile =
  "../../ATS/GIOP_client.ttcn3";

  static final boolean DEBUG = true;

  abstract class Type_1 extends Function {
    public abstract OctetString apply();

    public OctetString apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  public final OctetString TTCN3_REQUEST =
  new OctetString() {
    protected void init() {
      super.init();
      this.setLength(1);
      this.setItem(0, new Octet(0));
      this.toValue();
    }
  };

  public final OctetString TTCN3_REPLY =
  new OctetString() {
    protected void init() {
      super.init();
      this.setLength(1);
      this.setItem(0, new Octet(1));
      this.toValue();
    }
  };

  public final OctetString TTCN3_CANCELREQUEST =
  new OctetString() {
    protected void init() {
      super.init();
      this.setLength(1);
      this.setItem(0, new Octet(2));
      this.toValue();
    }
  };

  public final OctetString TTCN3_LOCATEREQUEST =
  new OctetString() {
    protected void init() {
      super.init();
      this.setLength(1);
      this.setItem(0, new Octet(3));
      this.toValue();
    }
  };

  public final OctetString TTCN3_LOCATEREPLY =
  new OctetString() {
    protected void init() {
      super.init();
      this.setLength(1);
      this.setItem(0, new Octet(4));
      this.toValue();
    }
  };

  public final OctetString TTCN3_CLOSECONNECTION =
  new OctetString() {
    protected void init() {
      super.init();
      this.setLength(1);
      this.setItem(0, new Octet(5));
      this.toValue();
    }
  };

  public final OctetString TTCN3_MESSAGEERROR =
  new OctetString() {
    protected void init() {
      super.init();
      this.setLength(1);
      this.setItem(0, new Octet(6));
      this.toValue();
    }
  };

  public final OctetString TTCN3_FRAGMENT =
  new OctetString() {
    protected void init() {
      super.init();
      this.setLength(1);
      this.setItem(0, new Octet(7));
      this.toValue();
    }
  };

  public final OctetString TTCN3_WRONGMESSAGETYPE =
  new OctetString() {
    protected void init() {
      super.init();
      this.setLength(1);
      this.setItem(0, new Octet(8));
      this.toValue();
    }
  };

  public final TTCN3_CORBA_ULong TTCN3_NO_EXCEPTION =
  (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(new Int(0));

  public final TTCN3_CORBA_ULong TTCN3_UNKNOWN_OBJECT =
  (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(new Int(0));

  public final TTCN3_CORBA_ULong TTCN3_OBJECT_HERE =
  (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(new Int(1));

  public final TTCN3_CORBA_ULong TTCN3_OBJECT_FORWARD =
  (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(new Int(2));

  public final TTCN3_CORBA_ULong TTCN3_OBJECT_FORWARD_PERM =
  (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(new Int(3));

  public final TTCN3_CORBA_ULong TTCN3_LOC_SYSTEM_EXCEPTION =
  (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(new Int(4));

  public final TTCN3_CORBA_ULong TTCN3_LOC_NEEDS_ADDRESSING_MODE =
  (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(new Int(5));

  public Type type_TTCN3_CORBA_ULong =
  new
    AbstractType(TTCN3_GIOP_client,
                 "CORBA_ULong",
                 Type.INTEGER,
                 TTCN3_CORBA_ULong.class) {
    public Value newInstance() {
      return new TTCN3_CORBA_ULong();
    }
  };

  public class TTCN3_CORBA_ULong extends Int {
    public Type getType() {
      return type_TTCN3_CORBA_ULong;
    }
  }

  public Type type_TTCN3_CORBA_Long =
  new
    AbstractType(TTCN3_GIOP_client,
                 "CORBA_Long",
                 Type.INTEGER,
                 TTCN3_CORBA_Long.class) {
    public Value newInstance() {
      return new TTCN3_CORBA_Long();
    }
  };

  public class TTCN3_CORBA_Long extends Int {
    public Type getType() {
      return type_TTCN3_CORBA_Long;
    }
  }

  public Type type_TTCN3_CORBA_LongLong =
  new
    AbstractType(TTCN3_GIOP_client,
                 "CORBA_LongLong",
                 Type.INTEGER,
                 TTCN3_CORBA_LongLong.class) {
    public Value newInstance() {
      return new TTCN3_CORBA_LongLong();
    }
  };

  public class TTCN3_CORBA_LongLong extends Int {
    public Type getType() {
      return type_TTCN3_CORBA_LongLong;
    }
  }

  public Type type_TTCN3_CORBA_UShort =
  new
    AbstractType(TTCN3_GIOP_client,
                 "CORBA_UShort",
                 Type.INTEGER,
                 TTCN3_CORBA_UShort.class) {
    public Value newInstance() {
      return new TTCN3_CORBA_UShort();
    }
  };

  public class TTCN3_CORBA_UShort extends Int {
    public Type getType() {
      return type_TTCN3_CORBA_UShort;
    }
  }

  public Type type_TTCN3_CORBA_Short =
  new
    AbstractType(TTCN3_GIOP_client,
                 "CORBA_Short",
                 Type.INTEGER,
                 TTCN3_CORBA_Short.class) {
    public Value newInstance() {
      return new TTCN3_CORBA_Short();
    }
  };

  public class TTCN3_CORBA_Short extends Int {
    public Type getType() {
      return type_TTCN3_CORBA_Short;
    }
  }

  public Type type_TTCN3_CORBA_Octet =
  new
    AbstractType(TTCN3_GIOP_client,
                 "CORBA_Octet",
                 Type.OCTETSTRING,
                 TTCN3_CORBA_Octet.class) {
    public Value newInstance() {
      return new TTCN3_CORBA_Octet();
    }
  };

  class TTCN3_CORBA_Octet extends OctetString {
    public Type getType() {
      return type_TTCN3_CORBA_Octet;
    }

    private final OctetString constraint =
      new OctetString() {
        public boolean matches(Data value) {
          if (!super.matches(value)) { return false; }
          if (!new Int(1).matches(new Int(((List)value).getLength()))) {
            return false;
          }
          return true;
        }
      };

    public boolean isValidValue() {
      return constraint.matches(this);
    }

    public boolean matches(Data value) {
      return constraint.matches(value);
    }

    public void setValue(Data value) {
      super.setValue(value);
      if (!isValidValue()) {
        throw new TestCaseError("invalid assignment: " + value);
      }
    }

    protected void init() {
      super.init();
      setLength(new Int(1));
    }
  }

  public Type type_TTCN3_CORBA_Float =
  new
    AbstractType(TTCN3_GIOP_client,
                 "CORBA_Float",
                 Type.FLOAT,
                 TTCN3_CORBA_Float.class) {
    public Value newInstance() {
      return new TTCN3_CORBA_Float();
    }
  };

  public class TTCN3_CORBA_Float extends Float {
    public Type getType() {
      return type_TTCN3_CORBA_Float;
    }
  }

  public Type type_TTCN3_CORBA_Double =
  new
    AbstractType(TTCN3_GIOP_client,
                 "CORBA_Double",
                 Type.FLOAT,
                 TTCN3_CORBA_Double.class) {
    public Value newInstance() {
      return new TTCN3_CORBA_Double();
    }
  };

  public class TTCN3_CORBA_Double extends Float {
    public Type getType() {
      return type_TTCN3_CORBA_Double;
    }
  }

  public Type type_TTCN3_CORBA_Char =
  new
    AbstractType(TTCN3_GIOP_client,
                 "CORBA_Char",
                 Type.CHARSTRING,
                 TTCN3_CORBA_Char.class) {
    public Value newInstance() {
      return new TTCN3_CORBA_Char();
    }
  };

  class TTCN3_CORBA_Char extends CharString {
    public Type getType() {
      return type_TTCN3_CORBA_Char;
    }

    private final CharString constraint =
      new CharString() {
        public boolean matches(Data value) {
          if (!super.matches(value)) { return false; }
          if (!new Int(1).matches(new Int(((List)value).getLength()))) {
            return false;
          }
          return true;
        }
      };

    public boolean isValidValue() {
      return constraint.matches(this);
    }

    public boolean matches(Data value) {
      return constraint.matches(value);
    }

    public void setValue(Data value) {
      super.setValue(value);
      if (!isValidValue()) {
        throw new TestCaseError("invalid assignment: " + value);
      }
    }

    protected void init() {
      super.init();
      setLength(new Int(1));
    }
  }

  public Type type_TTCN3_CORBA_String =
  new
    AbstractType(TTCN3_GIOP_client,
                 "CORBA_String",
                 Type.CHARSTRING,
                 TTCN3_CORBA_String.class) {
    public Value newInstance() {
      return new TTCN3_CORBA_String();
    }
  };

  class TTCN3_CORBA_String extends CharString {
    public Type getType() {
      return type_TTCN3_CORBA_String;
    }

    private final CharString constraint =
      new CharString() {
        public boolean matches(Data value) {
          if (!super.matches(value)) { return false; }
          if (!new Int(6).matches(new Int(((List)value).getLength()))) {
            return false;
          }
          return true;
        }
      };

    public boolean isValidValue() {
      return constraint.matches(this);
    }

    public boolean matches(Data value) {
      return constraint.matches(value);
    }

    public void setValue(Data value) {
      super.setValue(value);
      if (!isValidValue()) {
        throw new TestCaseError("invalid assignment: " + value);
      }
    }

    protected void init() {
      super.init();
      setLength(new Int(6));
    }
  }

  public Type type_TTCN3_CORBA_Enum =
  new
    AbstractType(TTCN3_GIOP_client,
                 "CORBA_Enum",
                 Type.INTEGER,
                 TTCN3_CORBA_Enum.class) {
    public Value newInstance() {
      return new TTCN3_CORBA_Enum();
    }
  };

  public class TTCN3_CORBA_Enum extends Int {
    public Type getType() {
      return type_TTCN3_CORBA_Enum;
    }
  }

  public Type type_Type_2 =
  new
    AbstractType(TTCN3_GIOP_client, "Type_2", Type.OCTETSTRING, Type_2.class) {
    public Value newInstance() {
      return new Type_2();
    }
  };

  class Type_2 extends OctetString {
    public Type getType() {
      return type_Type_2;
    }

    private final OctetString constraint =
      new OctetString() {
        public boolean matches(Data value) {
          if (!super.matches(value)) { return false; }
          if (!new Int(2).matches(new Int(((List)value).getLength()))) {
            return false;
          }
          return true;
        }
      };

    public boolean isValidValue() {
      return constraint.matches(this);
    }

    public boolean matches(Data value) {
      return constraint.matches(value);
    }

    public void setValue(Data value) {
      super.setValue(value);
      if (!isValidValue()) {
        throw new TestCaseError("invalid assignment: " + value);
      }
    }

    protected void init() {
      super.init();
      setLength(new Int(2));
    }
  }

  public Type type_TTCN3_TaggedProfile_IIOP =
  new
    AbstractType(TTCN3_GIOP_client,
                 "TaggedProfile_IIOP",
                 Type.RECORD,
                 TTCN3_TaggedProfile_IIOP.class) {
    public Value newInstance() {
      return new TTCN3_TaggedProfile_IIOP();
    }
  };

  public class TTCN3_TaggedProfile_IIOP extends Record {
    private TTCN3_CORBA_ULong TTCN3_tag;

    public TTCN3_CORBA_ULong get_TTCN3_tag() {
      this.lazyInit();
      return this.TTCN3_tag;
    }

    public void set_TTCN3_tag(Data TTCN3_tag) {
      this.lazyInit();
      this.TTCN3_tag.set(TTCN3_tag);
    }

    private Type_2 TTCN3_version;

    public Type_2 get_TTCN3_version() {
      this.lazyInit();
      return this.TTCN3_version;
    }

    public void set_TTCN3_version(Data TTCN3_version) {
      this.lazyInit();
      this.TTCN3_version.set(TTCN3_version);
    }

    private CharString TTCN3_host;

    public CharString get_TTCN3_host() {
      this.lazyInit();
      return this.TTCN3_host;
    }

    public void set_TTCN3_host(Data TTCN3_host) {
      this.lazyInit();
      this.TTCN3_host.set(TTCN3_host);
    }

    private TTCN3_CORBA_UShort TTCN3_tcp_port;

    public TTCN3_CORBA_UShort get_TTCN3_tcp_port() {
      this.lazyInit();
      return this.TTCN3_tcp_port;
    }

    public void set_TTCN3_tcp_port(Data TTCN3_tcp_port) {
      this.lazyInit();
      this.TTCN3_tcp_port.set(TTCN3_tcp_port);
    }

    private OctetString TTCN3_object_key;

    public OctetString get_TTCN3_object_key() {
      this.lazyInit();
      return this.TTCN3_object_key;
    }

    public void set_TTCN3_object_key(Data TTCN3_object_key) {
      this.lazyInit();
      this.TTCN3_object_key.set(TTCN3_object_key);
    }

    public Type getType() {
      return type_TTCN3_TaggedProfile_IIOP;
    }

    protected void init() {
      super.init();
      this.TTCN3_tag = new TTCN3_CORBA_ULong();
      this.addField("tag", TTCN3_tag);
      this.TTCN3_version = new Type_2();
      this.addField("version", TTCN3_version);
      this.TTCN3_host = new CharString();
      this.addField("host", TTCN3_host);
      this.TTCN3_tcp_port = new TTCN3_CORBA_UShort();
      this.addField("tcp_port", TTCN3_tcp_port);
      this.TTCN3_object_key = new OctetString();
      this.addField("object_key", TTCN3_object_key);
    }

    public void setValue(TTCN3_TaggedProfile_IIOP value) {
      this.set_TTCN3_tag(value.get_TTCN3_tag());
      this.set_TTCN3_version(value.get_TTCN3_version());
      this.set_TTCN3_host(value.get_TTCN3_host());
      this.set_TTCN3_tcp_port(value.get_TTCN3_tcp_port());
      this.set_TTCN3_object_key(value.get_TTCN3_object_key());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_TaggedProfile_IIOP)value));
    }

    public boolean matchesValue(TTCN3_TaggedProfile_IIOP value) {
      if (!get_TTCN3_tag().matches(value.get_TTCN3_tag())) { return false; }
      if (!get_TTCN3_version().matches(value.get_TTCN3_version())) {
        return false;
      }
      if (!get_TTCN3_host().matches(value.get_TTCN3_host())) { return false; }
      if (!get_TTCN3_tcp_port().matches(value.get_TTCN3_tcp_port())) {
        return false;
      }
      if (!get_TTCN3_object_key().matches(value.get_TTCN3_object_key())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_TaggedProfile_IIOP) {
        return this.matchesValue((TTCN3_TaggedProfile_IIOP)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_IOPProfiles =
  new
    AbstractType(TTCN3_GIOP_client,
                 "IOPProfiles",
                 Type.RECORD,
                 TTCN3_IOPProfiles.class) {
    public Value newInstance() {
      return new TTCN3_IOPProfiles();
    }
  };

  public class TTCN3_IOPProfiles extends Record {
    private TTCN3_TaggedProfile_IIOP TTCN3_profile_iiop;

    public TTCN3_TaggedProfile_IIOP get_TTCN3_profile_iiop() {
      this.lazyInit();
      return this.TTCN3_profile_iiop;
    }

    public void set_TTCN3_profile_iiop(Data TTCN3_profile_iiop) {
      this.lazyInit();
      this.TTCN3_profile_iiop.set(TTCN3_profile_iiop);
    }

    public Type getType() {
      return type_TTCN3_IOPProfiles;
    }

    protected void init() {
      super.init();
      this.TTCN3_profile_iiop = new TTCN3_TaggedProfile_IIOP();
      this.addField("profile_iiop", TTCN3_profile_iiop);
    }

    public void setValue(TTCN3_IOPProfiles value) {
      this.set_TTCN3_profile_iiop(value.get_TTCN3_profile_iiop());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_IOPProfiles)value));
    }

    public boolean matchesValue(TTCN3_IOPProfiles value) {
      if (!get_TTCN3_profile_iiop().matches(value.get_TTCN3_profile_iiop())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_IOPProfiles) {
        return this.matchesValue((TTCN3_IOPProfiles)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_IOR_IIOP =
  new
    AbstractType(TTCN3_GIOP_client,
                 "IOR_IIOP",
                 Type.RECORD,
                 TTCN3_IOR_IIOP.class) {
    public Value newInstance() {
      return new TTCN3_IOR_IIOP();
    }
  };

  public class TTCN3_IOR_IIOP extends Record {
    private CharString TTCN3_type_id;

    public CharString get_TTCN3_type_id() {
      this.lazyInit();
      return this.TTCN3_type_id;
    }

    public void set_TTCN3_type_id(Data TTCN3_type_id) {
      this.lazyInit();
      this.TTCN3_type_id.set(TTCN3_type_id);
    }

    private TTCN3_IOPProfiles TTCN3_profiles;

    public TTCN3_IOPProfiles get_TTCN3_profiles() {
      this.lazyInit();
      return this.TTCN3_profiles;
    }

    public void set_TTCN3_profiles(Data TTCN3_profiles) {
      this.lazyInit();
      this.TTCN3_profiles.set(TTCN3_profiles);
    }

    public Type getType() {
      return type_TTCN3_IOR_IIOP;
    }

    protected void init() {
      super.init();
      this.TTCN3_type_id = new CharString();
      this.addField("type_id", TTCN3_type_id);
      this.TTCN3_profiles = new TTCN3_IOPProfiles();
      this.addField("profiles", TTCN3_profiles);
    }

    public void setValue(TTCN3_IOR_IIOP value) {
      this.set_TTCN3_type_id(value.get_TTCN3_type_id());
      this.set_TTCN3_profiles(value.get_TTCN3_profiles());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_IOR_IIOP)value));
    }

    public boolean matchesValue(TTCN3_IOR_IIOP value) {
      if (!get_TTCN3_type_id().matches(value.get_TTCN3_type_id())) {
        return false;
      }
      if (!get_TTCN3_profiles().matches(value.get_TTCN3_profiles())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_IOR_IIOP) {
        return this.matchesValue((TTCN3_IOR_IIOP)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_TargetAddress_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "TargetAddress_Type",
                 Type.UNION,
                 TTCN3_TargetAddress_Type.class) {
    public Value newInstance() {
      return new TTCN3_TargetAddress_Type();
    }
  };

  public class TTCN3_TargetAddress_Type extends Union {
    protected void init() {
      super.init();
      this.addVariant("object_key", OctetString.type);
      this.addVariant("profile", type_TTCN3_TaggedProfile_IIOP);
      this.addVariant("ior", type_TTCN3_IOR_IIOP);
    }

    public Type getType() {
      return type_TTCN3_TargetAddress_Type;
    }

    public OctetString get_TTCN3_object_key() {
      return ((OctetString)getVariant("object_key"));
    }

    public void set_TTCN3_object_key(Data value) {
      setVariant("object_key", value);
    }

    public boolean is_TTCN3_object_key() {
      this.lazyInit();
      return isVariant("object_key");
    }

    public TTCN3_TaggedProfile_IIOP get_TTCN3_profile() {
      return ((TTCN3_TaggedProfile_IIOP)getVariant("profile"));
    }

    public void set_TTCN3_profile(Data value) {
      setVariant("profile", value);
    }

    public boolean is_TTCN3_profile() {
      this.lazyInit();
      return isVariant("profile");
    }

    public TTCN3_IOR_IIOP get_TTCN3_ior() {
      return ((TTCN3_IOR_IIOP)getVariant("ior"));
    }

    public void set_TTCN3_ior(Data value) {
      setVariant("ior", value);
    }

    public boolean is_TTCN3_ior() {
      this.lazyInit();
      return isVariant("ior");
    }
  }

  public Type type_Type_3 =
  new
    AbstractType(TTCN3_GIOP_client, "Type_3", Type.CHARSTRING, Type_3.class) {
    public Value newInstance() {
      return new Type_3();
    }
  };

  class Type_3 extends CharString {
    public Type getType() {
      return type_Type_3;
    }

    private final CharString constraint =
      new CharString() {
        public boolean matches(Data value) {
          if (!super.matches(value)) { return false; }
          if (!new Int(4).matches(new Int(((List)value).getLength()))) {
            return false;
          }
          return true;
        }
      };

    public boolean isValidValue() {
      return constraint.matches(this);
    }

    public boolean matches(Data value) {
      return constraint.matches(value);
    }

    public void setValue(Data value) {
      super.setValue(value);
      if (!isValidValue()) {
        throw new TestCaseError("invalid assignment: " + value);
      }
    }

    protected void init() {
      super.init();
      setLength(new Int(4));
    }
  }

  public Type type_Type_4 =
  new
    AbstractType(TTCN3_GIOP_client, "Type_4", Type.OCTETSTRING, Type_4.class) {
    public Value newInstance() {
      return new Type_4();
    }
  };

  class Type_4 extends OctetString {
    public Type getType() {
      return type_Type_4;
    }

    private final OctetString constraint =
      new OctetString() {
        public boolean matches(Data value) {
          if (!super.matches(value)) { return false; }
          if (!new Int(2).matches(new Int(((List)value).getLength()))) {
            return false;
          }
          return true;
        }
      };

    public boolean isValidValue() {
      return constraint.matches(this);
    }

    public boolean matches(Data value) {
      return constraint.matches(value);
    }

    public void setValue(Data value) {
      super.setValue(value);
      if (!isValidValue()) {
        throw new TestCaseError("invalid assignment: " + value);
      }
    }

    protected void init() {
      super.init();
      setLength(new Int(2));
    }
  }

  public Type type_TTCN3_GIOP_Header_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Header_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Header_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Header_Type();
    }
  };

  public class TTCN3_GIOP_Header_Type extends Record {
    private Type_3 TTCN3_magic;

    public Type_3 get_TTCN3_magic() {
      this.lazyInit();
      return this.TTCN3_magic;
    }

    public void set_TTCN3_magic(Data TTCN3_magic) {
      this.lazyInit();
      this.TTCN3_magic.set(TTCN3_magic);
    }

    private Type_4 TTCN3_version;

    public Type_4 get_TTCN3_version() {
      this.lazyInit();
      return this.TTCN3_version;
    }

    public void set_TTCN3_version(Data TTCN3_version) {
      this.lazyInit();
      this.TTCN3_version.set(TTCN3_version);
    }

    private TTCN3_CORBA_Octet TTCN3_byteorder;

    public TTCN3_CORBA_Octet get_TTCN3_byteorder() {
      this.lazyInit();
      return this.TTCN3_byteorder;
    }

    public void set_TTCN3_byteorder(Data TTCN3_byteorder) {
      this.lazyInit();
      this.TTCN3_byteorder.set(TTCN3_byteorder);
    }

    private OctetString TTCN3_messagetype;

    public OctetString get_TTCN3_messagetype() {
      this.lazyInit();
      return this.TTCN3_messagetype;
    }

    public void set_TTCN3_messagetype(Data TTCN3_messagetype) {
      this.lazyInit();
      this.TTCN3_messagetype.set(TTCN3_messagetype);
    }

    private TTCN3_CORBA_ULong TTCN3_messagesize;

    public TTCN3_CORBA_ULong get_TTCN3_messagesize() {
      this.lazyInit();
      return this.TTCN3_messagesize;
    }

    public void set_TTCN3_messagesize(Data TTCN3_messagesize) {
      this.lazyInit();
      this.TTCN3_messagesize.set(TTCN3_messagesize);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Header_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_magic = new Type_3();
      this.addField("magic", TTCN3_magic);
      this.TTCN3_version = new Type_4();
      this.addField("version", TTCN3_version);
      this.TTCN3_byteorder = new TTCN3_CORBA_Octet();
      this.addField("byteorder", TTCN3_byteorder);
      this.TTCN3_messagetype = new OctetString();
      this.addField("messagetype", TTCN3_messagetype);
      this.TTCN3_messagesize = new TTCN3_CORBA_ULong();
      this.addField("messagesize", TTCN3_messagesize);
    }

    public void setValue(TTCN3_GIOP_Header_Type value) {
      this.set_TTCN3_magic(value.get_TTCN3_magic());
      this.set_TTCN3_version(value.get_TTCN3_version());
      this.set_TTCN3_byteorder(value.get_TTCN3_byteorder());
      this.set_TTCN3_messagetype(value.get_TTCN3_messagetype());
      this.set_TTCN3_messagesize(value.get_TTCN3_messagesize());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Header_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Header_Type value) {
      if (!get_TTCN3_magic().matches(value.get_TTCN3_magic())) {
        return false;
      }
      if (!get_TTCN3_version().matches(value.get_TTCN3_version())) {
        return false;
      }
      if (!get_TTCN3_byteorder().matches(value.get_TTCN3_byteorder())) {
        return false;
      }
      if (!get_TTCN3_messagetype().matches(value.get_TTCN3_messagetype())) {
        return false;
      }
      if (!get_TTCN3_messagesize().matches(value.get_TTCN3_messagesize())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Header_Type) {
        return this.matchesValue((TTCN3_GIOP_Header_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_Type_5 =
  new
    AbstractType(TTCN3_GIOP_client, "Type_5", Type.OCTETSTRING, Type_5.class) {
    public Value newInstance() {
      return new Type_5();
    }
  };

  class Type_5 extends OctetString {
    public Type getType() {
      return type_Type_5;
    }

    private final OctetString constraint =
      new OctetString() {
        public boolean matches(Data value) {
          if (!super.matches(value)) { return false; }
          if (!new Int(3).matches(new Int(((List)value).getLength()))) {
            return false;
          }
          return true;
        }
      };

    public boolean isValidValue() {
      return constraint.matches(this);
    }

    public boolean matches(Data value) {
      return constraint.matches(value);
    }

    public void setValue(Data value) {
      super.setValue(value);
      if (!isValidValue()) {
        throw new TestCaseError("invalid assignment: " + value);
      }
    }

    protected void init() {
      super.init();
      setLength(new Int(3));
    }
  }

  public Type type_TTCN3_Request_Header_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Request_Header_Type",
                 Type.RECORD,
                 TTCN3_Request_Header_Type.class) {
    public Value newInstance() {
      return new TTCN3_Request_Header_Type();
    }
  };

  public class TTCN3_Request_Header_Type extends Record {
    private TTCN3_CORBA_ULong TTCN3_request_id;

    public TTCN3_CORBA_ULong get_TTCN3_request_id() {
      this.lazyInit();
      return this.TTCN3_request_id;
    }

    public void set_TTCN3_request_id(Data TTCN3_request_id) {
      this.lazyInit();
      this.TTCN3_request_id.set(TTCN3_request_id);
    }

    private TTCN3_CORBA_Octet TTCN3_response_flags;

    public TTCN3_CORBA_Octet get_TTCN3_response_flags() {
      this.lazyInit();
      return this.TTCN3_response_flags;
    }

    public void set_TTCN3_response_flags(Data TTCN3_response_flags) {
      this.lazyInit();
      this.TTCN3_response_flags.set(TTCN3_response_flags);
    }

    private Type_5 TTCN3_reserved;

    public Type_5 get_TTCN3_reserved() {
      this.lazyInit();
      return this.TTCN3_reserved;
    }

    public void set_TTCN3_reserved(Data TTCN3_reserved) {
      this.lazyInit();
      this.TTCN3_reserved.set(TTCN3_reserved);
    }

    private TTCN3_CORBA_Short TTCN3_addressing_disposition;

    public TTCN3_CORBA_Short get_TTCN3_addressing_disposition() {
      this.lazyInit();
      return this.TTCN3_addressing_disposition;
    }

    public
      void
        set_TTCN3_addressing_disposition(Data TTCN3_addressing_disposition) {
      this.lazyInit();
      this.TTCN3_addressing_disposition.set(TTCN3_addressing_disposition);
    }

    private TTCN3_TargetAddress_Type TTCN3_target;

    public TTCN3_TargetAddress_Type get_TTCN3_target() {
      this.lazyInit();
      return this.TTCN3_target;
    }

    public void set_TTCN3_target(Data TTCN3_target) {
      this.lazyInit();
      this.TTCN3_target.set(TTCN3_target);
    }

    private CharString TTCN3_operation;

    public CharString get_TTCN3_operation() {
      this.lazyInit();
      return this.TTCN3_operation;
    }

    public void set_TTCN3_operation(Data TTCN3_operation) {
      this.lazyInit();
      this.TTCN3_operation.set(TTCN3_operation);
    }

    private TTCN3_CORBA_ULong TTCN3_service_context;

    public TTCN3_CORBA_ULong get_TTCN3_service_context() {
      this.lazyInit();
      return this.TTCN3_service_context;
    }

    public void set_TTCN3_service_context(Data TTCN3_service_context) {
      this.lazyInit();
      this.TTCN3_service_context.set(TTCN3_service_context);
    }

    public Type getType() {
      return type_TTCN3_Request_Header_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_request_id = new TTCN3_CORBA_ULong();
      this.addField("request_id", TTCN3_request_id);
      this.TTCN3_response_flags = new TTCN3_CORBA_Octet();
      this.addField("response_flags", TTCN3_response_flags);
      this.TTCN3_reserved = new Type_5();
      this.addField("reserved", TTCN3_reserved);
      this.TTCN3_addressing_disposition = new TTCN3_CORBA_Short();
      this.addField("addressing_disposition", TTCN3_addressing_disposition);
      this.TTCN3_target = new TTCN3_TargetAddress_Type();
      this.addField("target", TTCN3_target);
      this.TTCN3_operation = new CharString();
      this.addField("operation", TTCN3_operation);
      this.TTCN3_service_context = new TTCN3_CORBA_ULong();
      this.addField("service_context", TTCN3_service_context);
    }

    public void setValue(TTCN3_Request_Header_Type value) {
      this.set_TTCN3_request_id(value.get_TTCN3_request_id());
      this.set_TTCN3_response_flags(value.get_TTCN3_response_flags());
      this.set_TTCN3_reserved(value.get_TTCN3_reserved());
      this
      .set_TTCN3_addressing_disposition(                                                                                                                                                                                value
                                        .get_TTCN3_addressing_disposition());
      this.set_TTCN3_target(value.get_TTCN3_target());
      this.set_TTCN3_operation(value.get_TTCN3_operation());
      this.set_TTCN3_service_context(value.get_TTCN3_service_context());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_Request_Header_Type)value));
    }

    public boolean matchesValue(TTCN3_Request_Header_Type value) {
      if (!get_TTCN3_request_id().matches(value.get_TTCN3_request_id())) {
        return false;
      }
      if (!get_TTCN3_response_flags()
           .matches(value.get_TTCN3_response_flags())) {
        return false;
      }
      if (!get_TTCN3_reserved().matches(value.get_TTCN3_reserved())) {
        return false;
      }
      if (!get_TTCN3_addressing_disposition()
           .matches(value.get_TTCN3_addressing_disposition())) {
        return false;
      }
      if (!get_TTCN3_target().matches(value.get_TTCN3_target())) {
        return false;
      }
      if (!get_TTCN3_operation().matches(value.get_TTCN3_operation())) {
        return false;
      }
      if (!get_TTCN3_service_context()
           .matches(value.get_TTCN3_service_context())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_Request_Header_Type) {
        return this.matchesValue((TTCN3_Request_Header_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_Reply_Header_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Reply_Header_Type",
                 Type.RECORD,
                 TTCN3_Reply_Header_Type.class) {
    public Value newInstance() {
      return new TTCN3_Reply_Header_Type();
    }
  };

  public class TTCN3_Reply_Header_Type extends Record {
    private TTCN3_CORBA_ULong TTCN3_request_id;

    public TTCN3_CORBA_ULong get_TTCN3_request_id() {
      this.lazyInit();
      return this.TTCN3_request_id;
    }

    public void set_TTCN3_request_id(Data TTCN3_request_id) {
      this.lazyInit();
      this.TTCN3_request_id.set(TTCN3_request_id);
    }

    private TTCN3_CORBA_ULong TTCN3_reply_status;

    public TTCN3_CORBA_ULong get_TTCN3_reply_status() {
      this.lazyInit();
      return this.TTCN3_reply_status;
    }

    public void set_TTCN3_reply_status(Data TTCN3_reply_status) {
      this.lazyInit();
      this.TTCN3_reply_status.set(TTCN3_reply_status);
    }

    private TTCN3_CORBA_ULong TTCN3_service_context;

    public TTCN3_CORBA_ULong get_TTCN3_service_context() {
      this.lazyInit();
      return this.TTCN3_service_context;
    }

    public void set_TTCN3_service_context(Data TTCN3_service_context) {
      this.lazyInit();
      this.TTCN3_service_context.set(TTCN3_service_context);
    }

    public Type getType() {
      return type_TTCN3_Reply_Header_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_request_id = new TTCN3_CORBA_ULong();
      this.addField("request_id", TTCN3_request_id);
      this.TTCN3_reply_status = new TTCN3_CORBA_ULong();
      this.addField("reply_status", TTCN3_reply_status);
      this.TTCN3_service_context = new TTCN3_CORBA_ULong();
      this.addField("service_context", TTCN3_service_context);
    }

    public void setValue(TTCN3_Reply_Header_Type value) {
      this.set_TTCN3_request_id(value.get_TTCN3_request_id());
      this.set_TTCN3_reply_status(value.get_TTCN3_reply_status());
      this.set_TTCN3_service_context(value.get_TTCN3_service_context());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_Reply_Header_Type)value));
    }

    public boolean matchesValue(TTCN3_Reply_Header_Type value) {
      if (!get_TTCN3_request_id().matches(value.get_TTCN3_request_id())) {
        return false;
      }
      if (!get_TTCN3_reply_status().matches(value.get_TTCN3_reply_status())) {
        return false;
      }
      if (!get_TTCN3_service_context()
           .matches(value.get_TTCN3_service_context())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_Reply_Header_Type) {
        return this.matchesValue((TTCN3_Reply_Header_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_LocateRequest_Header_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "LocateRequest_Header_Type",
                 Type.RECORD,
                 TTCN3_LocateRequest_Header_Type.class) {
    public Value newInstance() {
      return new TTCN3_LocateRequest_Header_Type();
    }
  };

  public class TTCN3_LocateRequest_Header_Type extends Record {
    private TTCN3_CORBA_ULong TTCN3_request_id;

    public TTCN3_CORBA_ULong get_TTCN3_request_id() {
      this.lazyInit();
      return this.TTCN3_request_id;
    }

    public void set_TTCN3_request_id(Data TTCN3_request_id) {
      this.lazyInit();
      this.TTCN3_request_id.set(TTCN3_request_id);
    }

    private TTCN3_CORBA_Short TTCN3_addressing_disposition;

    public TTCN3_CORBA_Short get_TTCN3_addressing_disposition() {
      this.lazyInit();
      return this.TTCN3_addressing_disposition;
    }

    public
      void
        set_TTCN3_addressing_disposition(Data TTCN3_addressing_disposition) {
      this.lazyInit();
      this.TTCN3_addressing_disposition.set(TTCN3_addressing_disposition);
    }

    private TTCN3_TargetAddress_Type TTCN3_target;

    public TTCN3_TargetAddress_Type get_TTCN3_target() {
      this.lazyInit();
      return this.TTCN3_target;
    }

    public void set_TTCN3_target(Data TTCN3_target) {
      this.lazyInit();
      this.TTCN3_target.set(TTCN3_target);
    }

    public Type getType() {
      return type_TTCN3_LocateRequest_Header_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_request_id = new TTCN3_CORBA_ULong();
      this.addField("request_id", TTCN3_request_id);
      this.TTCN3_addressing_disposition = new TTCN3_CORBA_Short();
      this.addField("addressing_disposition", TTCN3_addressing_disposition);
      this.TTCN3_target = new TTCN3_TargetAddress_Type();
      this.addField("target", TTCN3_target);
    }

    public void setValue(TTCN3_LocateRequest_Header_Type value) {
      this.set_TTCN3_request_id(value.get_TTCN3_request_id());
      this
      .set_TTCN3_addressing_disposition(                                                                                                                                                                                value
                                        .get_TTCN3_addressing_disposition());
      this.set_TTCN3_target(value.get_TTCN3_target());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_LocateRequest_Header_Type)value));
    }

    public boolean matchesValue(TTCN3_LocateRequest_Header_Type value) {
      if (!get_TTCN3_request_id().matches(value.get_TTCN3_request_id())) {
        return false;
      }
      if (!get_TTCN3_addressing_disposition()
           .matches(value.get_TTCN3_addressing_disposition())) {
        return false;
      }
      if (!get_TTCN3_target().matches(value.get_TTCN3_target())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_LocateRequest_Header_Type) {
        return this.matchesValue((TTCN3_LocateRequest_Header_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_LocateReply_Header_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "LocateReply_Header_Type",
                 Type.RECORD,
                 TTCN3_LocateReply_Header_Type.class) {
    public Value newInstance() {
      return new TTCN3_LocateReply_Header_Type();
    }
  };

  public class TTCN3_LocateReply_Header_Type extends Record {
    private TTCN3_CORBA_ULong TTCN3_request_id;

    public TTCN3_CORBA_ULong get_TTCN3_request_id() {
      this.lazyInit();
      return this.TTCN3_request_id;
    }

    public void set_TTCN3_request_id(Data TTCN3_request_id) {
      this.lazyInit();
      this.TTCN3_request_id.set(TTCN3_request_id);
    }

    private TTCN3_CORBA_ULong TTCN3_locate_status;

    public TTCN3_CORBA_ULong get_TTCN3_locate_status() {
      this.lazyInit();
      return this.TTCN3_locate_status;
    }

    public void set_TTCN3_locate_status(Data TTCN3_locate_status) {
      this.lazyInit();
      this.TTCN3_locate_status.set(TTCN3_locate_status);
    }

    public Type getType() {
      return type_TTCN3_LocateReply_Header_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_request_id = new TTCN3_CORBA_ULong();
      this.addField("request_id", TTCN3_request_id);
      this.TTCN3_locate_status = new TTCN3_CORBA_ULong();
      this.addField("locate_status", TTCN3_locate_status);
    }

    public void setValue(TTCN3_LocateReply_Header_Type value) {
      this.set_TTCN3_request_id(value.get_TTCN3_request_id());
      this.set_TTCN3_locate_status(value.get_TTCN3_locate_status());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_LocateReply_Header_Type)value));
    }

    public boolean matchesValue(TTCN3_LocateReply_Header_Type value) {
      if (!get_TTCN3_request_id().matches(value.get_TTCN3_request_id())) {
        return false;
      }
      if (!get_TTCN3_locate_status()
           .matches(value.get_TTCN3_locate_status())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_LocateReply_Header_Type) {
        return this.matchesValue((TTCN3_LocateReply_Header_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_Reply_Body_Boolean_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Reply_Body_Boolean_Type",
                 Type.OCTETSTRING,
                 TTCN3_Reply_Body_Boolean_Type.class) {
    public Value newInstance() {
      return new TTCN3_Reply_Body_Boolean_Type();
    }
  };

  class TTCN3_Reply_Body_Boolean_Type extends OctetString {
    public Type getType() {
      return type_TTCN3_Reply_Body_Boolean_Type;
    }
  }

  public Type type_TTCN3_Reply_Body_Long_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Reply_Body_Long_Type",
                 Type.INTEGER,
                 TTCN3_Reply_Body_Long_Type.class) {
    public Value newInstance() {
      return new TTCN3_Reply_Body_Long_Type();
    }
  };

  public class TTCN3_Reply_Body_Long_Type extends Int {
    public Type getType() {
      return type_TTCN3_Reply_Body_Long_Type;
    }
  }

  public Type type_TTCN3_Reply_Body_LongLong_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Reply_Body_LongLong_Type",
                 Type.INTEGER,
                 TTCN3_Reply_Body_LongLong_Type.class) {
    public Value newInstance() {
      return new TTCN3_Reply_Body_LongLong_Type();
    }
  };

  public class TTCN3_Reply_Body_LongLong_Type extends Int {
    public Type getType() {
      return type_TTCN3_Reply_Body_LongLong_Type;
    }
  }

  public Type type_TTCN3_Reply_Body_Short_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Reply_Body_Short_Type",
                 Type.INTEGER,
                 TTCN3_Reply_Body_Short_Type.class) {
    public Value newInstance() {
      return new TTCN3_Reply_Body_Short_Type();
    }
  };

  public class TTCN3_Reply_Body_Short_Type extends Int {
    public Type getType() {
      return type_TTCN3_Reply_Body_Short_Type;
    }
  }

  public Type type_TTCN3_Reply_Body_Octet_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Reply_Body_Octet_Type",
                 Type.OCTETSTRING,
                 TTCN3_Reply_Body_Octet_Type.class) {
    public Value newInstance() {
      return new TTCN3_Reply_Body_Octet_Type();
    }
  };

  public class TTCN3_Reply_Body_Octet_Type extends TTCN3_CORBA_Octet {
    public Type getType() {
      return type_TTCN3_Reply_Body_Octet_Type;
    }
  }

  public Type type_TTCN3_Reply_Body_Float_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Reply_Body_Float_Type",
                 Type.FLOAT,
                 TTCN3_Reply_Body_Float_Type.class) {
    public Value newInstance() {
      return new TTCN3_Reply_Body_Float_Type();
    }
  };

  public class TTCN3_Reply_Body_Float_Type extends Float {
    public Type getType() {
      return type_TTCN3_Reply_Body_Float_Type;
    }
  }

  public Type type_TTCN3_Reply_Body_Double_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Reply_Body_Double_Type",
                 Type.FLOAT,
                 TTCN3_Reply_Body_Double_Type.class) {
    public Value newInstance() {
      return new TTCN3_Reply_Body_Double_Type();
    }
  };

  public class TTCN3_Reply_Body_Double_Type extends Float {
    public Type getType() {
      return type_TTCN3_Reply_Body_Double_Type;
    }
  }

  public Type type_TTCN3_Reply_Body_Char_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Reply_Body_Char_Type",
                 Type.CHARSTRING,
                 TTCN3_Reply_Body_Char_Type.class) {
    public Value newInstance() {
      return new TTCN3_Reply_Body_Char_Type();
    }
  };

  public class TTCN3_Reply_Body_Char_Type extends TTCN3_CORBA_Char {
    public Type getType() {
      return type_TTCN3_Reply_Body_Char_Type;
    }
  }

  public Type type_TTCN3_Reply_Body_String_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Reply_Body_String_Type",
                 Type.CHARSTRING,
                 TTCN3_Reply_Body_String_Type.class) {
    public Value newInstance() {
      return new TTCN3_Reply_Body_String_Type();
    }
  };

  public class TTCN3_Reply_Body_String_Type extends TTCN3_CORBA_String {
    public Type getType() {
      return type_TTCN3_Reply_Body_String_Type;
    }
  }

  public Type type_TTCN3_Reply_Body_Enum_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Reply_Body_Enum_Type",
                 Type.INTEGER,
                 TTCN3_Reply_Body_Enum_Type.class) {
    public Value newInstance() {
      return new TTCN3_Reply_Body_Enum_Type();
    }
  };

  public class TTCN3_Reply_Body_Enum_Type extends Int {
    public Type getType() {
      return type_TTCN3_Reply_Body_Enum_Type;
    }
  }

  public Type type_TTCN3_GIOP_Request_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Request_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Request_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Request_Type();
    }
  };

  public class TTCN3_GIOP_Request_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_Request_Header_Type TTCN3_request_header;

    public TTCN3_Request_Header_Type get_TTCN3_request_header() {
      this.lazyInit();
      return this.TTCN3_request_header;
    }

    public void set_TTCN3_request_header(Data TTCN3_request_header) {
      this.lazyInit();
      this.TTCN3_request_header.set(TTCN3_request_header);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Request_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_request_header = new TTCN3_Request_Header_Type();
      this.addField("request_header", TTCN3_request_header);
    }

    public void setValue(TTCN3_GIOP_Request_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_request_header(value.get_TTCN3_request_header());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Request_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Request_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_request_header()
           .matches(value.get_TTCN3_request_header())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Request_Type) {
        return this.matchesValue((TTCN3_GIOP_Request_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_Reply_Boolean_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Reply_Boolean_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Reply_Boolean_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Reply_Boolean_Type();
    }
  };

  public class TTCN3_GIOP_Reply_Boolean_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_Reply_Header_Type TTCN3_reply_header;

    public TTCN3_Reply_Header_Type get_TTCN3_reply_header() {
      this.lazyInit();
      return this.TTCN3_reply_header;
    }

    public void set_TTCN3_reply_header(Data TTCN3_reply_header) {
      this.lazyInit();
      this.TTCN3_reply_header.set(TTCN3_reply_header);
    }

    private TTCN3_Reply_Body_Boolean_Type TTCN3_reply_body;

    public TTCN3_Reply_Body_Boolean_Type get_TTCN3_reply_body() {
      this.lazyInit();
      return this.TTCN3_reply_body;
    }

    public void set_TTCN3_reply_body(Data TTCN3_reply_body) {
      this.lazyInit();
      this.TTCN3_reply_body.set(TTCN3_reply_body);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Reply_Boolean_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_reply_header = new TTCN3_Reply_Header_Type();
      this.addField("reply_header", TTCN3_reply_header);
      this.TTCN3_reply_body = new TTCN3_Reply_Body_Boolean_Type();
      this.addField("reply_body", TTCN3_reply_body);
    }

    public void setValue(TTCN3_GIOP_Reply_Boolean_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_reply_header(value.get_TTCN3_reply_header());
      this.set_TTCN3_reply_body(value.get_TTCN3_reply_body());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Reply_Boolean_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Reply_Boolean_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_reply_header().matches(value.get_TTCN3_reply_header())) {
        return false;
      }
      if (!get_TTCN3_reply_body().matches(value.get_TTCN3_reply_body())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Reply_Boolean_Type) {
        return this.matchesValue((TTCN3_GIOP_Reply_Boolean_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_Reply_Long_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Reply_Long_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Reply_Long_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Reply_Long_Type();
    }
  };

  public class TTCN3_GIOP_Reply_Long_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_Reply_Header_Type TTCN3_reply_header;

    public TTCN3_Reply_Header_Type get_TTCN3_reply_header() {
      this.lazyInit();
      return this.TTCN3_reply_header;
    }

    public void set_TTCN3_reply_header(Data TTCN3_reply_header) {
      this.lazyInit();
      this.TTCN3_reply_header.set(TTCN3_reply_header);
    }

    private TTCN3_Reply_Body_Long_Type TTCN3_reply_body;

    public TTCN3_Reply_Body_Long_Type get_TTCN3_reply_body() {
      this.lazyInit();
      return this.TTCN3_reply_body;
    }

    public void set_TTCN3_reply_body(Data TTCN3_reply_body) {
      this.lazyInit();
      this.TTCN3_reply_body.set(TTCN3_reply_body);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Reply_Long_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_reply_header = new TTCN3_Reply_Header_Type();
      this.addField("reply_header", TTCN3_reply_header);
      this.TTCN3_reply_body = new TTCN3_Reply_Body_Long_Type();
      this.addField("reply_body", TTCN3_reply_body);
    }

    public void setValue(TTCN3_GIOP_Reply_Long_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_reply_header(value.get_TTCN3_reply_header());
      this.set_TTCN3_reply_body(value.get_TTCN3_reply_body());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Reply_Long_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Reply_Long_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_reply_header().matches(value.get_TTCN3_reply_header())) {
        return false;
      }
      if (!get_TTCN3_reply_body().matches(value.get_TTCN3_reply_body())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Reply_Long_Type) {
        return this.matchesValue((TTCN3_GIOP_Reply_Long_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_Reply_LongLong_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Reply_LongLong_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Reply_LongLong_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Reply_LongLong_Type();
    }
  };

  public class TTCN3_GIOP_Reply_LongLong_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_Reply_Header_Type TTCN3_reply_header;

    public TTCN3_Reply_Header_Type get_TTCN3_reply_header() {
      this.lazyInit();
      return this.TTCN3_reply_header;
    }

    public void set_TTCN3_reply_header(Data TTCN3_reply_header) {
      this.lazyInit();
      this.TTCN3_reply_header.set(TTCN3_reply_header);
    }

    private TTCN3_Reply_Body_LongLong_Type TTCN3_reply_body;

    public TTCN3_Reply_Body_LongLong_Type get_TTCN3_reply_body() {
      this.lazyInit();
      return this.TTCN3_reply_body;
    }

    public void set_TTCN3_reply_body(Data TTCN3_reply_body) {
      this.lazyInit();
      this.TTCN3_reply_body.set(TTCN3_reply_body);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Reply_LongLong_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_reply_header = new TTCN3_Reply_Header_Type();
      this.addField("reply_header", TTCN3_reply_header);
      this.TTCN3_reply_body = new TTCN3_Reply_Body_LongLong_Type();
      this.addField("reply_body", TTCN3_reply_body);
    }

    public void setValue(TTCN3_GIOP_Reply_LongLong_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_reply_header(value.get_TTCN3_reply_header());
      this.set_TTCN3_reply_body(value.get_TTCN3_reply_body());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Reply_LongLong_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Reply_LongLong_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_reply_header().matches(value.get_TTCN3_reply_header())) {
        return false;
      }
      if (!get_TTCN3_reply_body().matches(value.get_TTCN3_reply_body())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Reply_LongLong_Type) {
        return this.matchesValue((TTCN3_GIOP_Reply_LongLong_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_Reply_Short_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Reply_Short_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Reply_Short_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Reply_Short_Type();
    }
  };

  public class TTCN3_GIOP_Reply_Short_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_Reply_Header_Type TTCN3_reply_header;

    public TTCN3_Reply_Header_Type get_TTCN3_reply_header() {
      this.lazyInit();
      return this.TTCN3_reply_header;
    }

    public void set_TTCN3_reply_header(Data TTCN3_reply_header) {
      this.lazyInit();
      this.TTCN3_reply_header.set(TTCN3_reply_header);
    }

    private TTCN3_Reply_Body_Short_Type TTCN3_reply_body;

    public TTCN3_Reply_Body_Short_Type get_TTCN3_reply_body() {
      this.lazyInit();
      return this.TTCN3_reply_body;
    }

    public void set_TTCN3_reply_body(Data TTCN3_reply_body) {
      this.lazyInit();
      this.TTCN3_reply_body.set(TTCN3_reply_body);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Reply_Short_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_reply_header = new TTCN3_Reply_Header_Type();
      this.addField("reply_header", TTCN3_reply_header);
      this.TTCN3_reply_body = new TTCN3_Reply_Body_Short_Type();
      this.addField("reply_body", TTCN3_reply_body);
    }

    public void setValue(TTCN3_GIOP_Reply_Short_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_reply_header(value.get_TTCN3_reply_header());
      this.set_TTCN3_reply_body(value.get_TTCN3_reply_body());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Reply_Short_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Reply_Short_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_reply_header().matches(value.get_TTCN3_reply_header())) {
        return false;
      }
      if (!get_TTCN3_reply_body().matches(value.get_TTCN3_reply_body())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Reply_Short_Type) {
        return this.matchesValue((TTCN3_GIOP_Reply_Short_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_Reply_Octet_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Reply_Octet_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Reply_Octet_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Reply_Octet_Type();
    }
  };

  public class TTCN3_GIOP_Reply_Octet_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_Reply_Header_Type TTCN3_reply_header;

    public TTCN3_Reply_Header_Type get_TTCN3_reply_header() {
      this.lazyInit();
      return this.TTCN3_reply_header;
    }

    public void set_TTCN3_reply_header(Data TTCN3_reply_header) {
      this.lazyInit();
      this.TTCN3_reply_header.set(TTCN3_reply_header);
    }

    private TTCN3_Reply_Body_Octet_Type TTCN3_reply_body;

    public TTCN3_Reply_Body_Octet_Type get_TTCN3_reply_body() {
      this.lazyInit();
      return this.TTCN3_reply_body;
    }

    public void set_TTCN3_reply_body(Data TTCN3_reply_body) {
      this.lazyInit();
      this.TTCN3_reply_body.set(TTCN3_reply_body);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Reply_Octet_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_reply_header = new TTCN3_Reply_Header_Type();
      this.addField("reply_header", TTCN3_reply_header);
      this.TTCN3_reply_body = new TTCN3_Reply_Body_Octet_Type();
      this.addField("reply_body", TTCN3_reply_body);
    }

    public void setValue(TTCN3_GIOP_Reply_Octet_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_reply_header(value.get_TTCN3_reply_header());
      this.set_TTCN3_reply_body(value.get_TTCN3_reply_body());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Reply_Octet_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Reply_Octet_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_reply_header().matches(value.get_TTCN3_reply_header())) {
        return false;
      }
      if (!get_TTCN3_reply_body().matches(value.get_TTCN3_reply_body())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Reply_Octet_Type) {
        return this.matchesValue((TTCN3_GIOP_Reply_Octet_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_Reply_Float_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Reply_Float_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Reply_Float_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Reply_Float_Type();
    }
  };

  public class TTCN3_GIOP_Reply_Float_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_Reply_Header_Type TTCN3_reply_header;

    public TTCN3_Reply_Header_Type get_TTCN3_reply_header() {
      this.lazyInit();
      return this.TTCN3_reply_header;
    }

    public void set_TTCN3_reply_header(Data TTCN3_reply_header) {
      this.lazyInit();
      this.TTCN3_reply_header.set(TTCN3_reply_header);
    }

    private TTCN3_Reply_Body_Float_Type TTCN3_reply_body;

    public TTCN3_Reply_Body_Float_Type get_TTCN3_reply_body() {
      this.lazyInit();
      return this.TTCN3_reply_body;
    }

    public void set_TTCN3_reply_body(Data TTCN3_reply_body) {
      this.lazyInit();
      this.TTCN3_reply_body.set(TTCN3_reply_body);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Reply_Float_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_reply_header = new TTCN3_Reply_Header_Type();
      this.addField("reply_header", TTCN3_reply_header);
      this.TTCN3_reply_body = new TTCN3_Reply_Body_Float_Type();
      this.addField("reply_body", TTCN3_reply_body);
    }

    public void setValue(TTCN3_GIOP_Reply_Float_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_reply_header(value.get_TTCN3_reply_header());
      this.set_TTCN3_reply_body(value.get_TTCN3_reply_body());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Reply_Float_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Reply_Float_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_reply_header().matches(value.get_TTCN3_reply_header())) {
        return false;
      }
      if (!get_TTCN3_reply_body().matches(value.get_TTCN3_reply_body())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Reply_Float_Type) {
        return this.matchesValue((TTCN3_GIOP_Reply_Float_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_Reply_Double_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Reply_Double_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Reply_Double_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Reply_Double_Type();
    }
  };

  public class TTCN3_GIOP_Reply_Double_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_Reply_Header_Type TTCN3_reply_header;

    public TTCN3_Reply_Header_Type get_TTCN3_reply_header() {
      this.lazyInit();
      return this.TTCN3_reply_header;
    }

    public void set_TTCN3_reply_header(Data TTCN3_reply_header) {
      this.lazyInit();
      this.TTCN3_reply_header.set(TTCN3_reply_header);
    }

    private TTCN3_Reply_Body_Double_Type TTCN3_reply_body;

    public TTCN3_Reply_Body_Double_Type get_TTCN3_reply_body() {
      this.lazyInit();
      return this.TTCN3_reply_body;
    }

    public void set_TTCN3_reply_body(Data TTCN3_reply_body) {
      this.lazyInit();
      this.TTCN3_reply_body.set(TTCN3_reply_body);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Reply_Double_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_reply_header = new TTCN3_Reply_Header_Type();
      this.addField("reply_header", TTCN3_reply_header);
      this.TTCN3_reply_body = new TTCN3_Reply_Body_Double_Type();
      this.addField("reply_body", TTCN3_reply_body);
    }

    public void setValue(TTCN3_GIOP_Reply_Double_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_reply_header(value.get_TTCN3_reply_header());
      this.set_TTCN3_reply_body(value.get_TTCN3_reply_body());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Reply_Double_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Reply_Double_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_reply_header().matches(value.get_TTCN3_reply_header())) {
        return false;
      }
      if (!get_TTCN3_reply_body().matches(value.get_TTCN3_reply_body())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Reply_Double_Type) {
        return this.matchesValue((TTCN3_GIOP_Reply_Double_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_Reply_Char_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Reply_Char_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Reply_Char_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Reply_Char_Type();
    }
  };

  public class TTCN3_GIOP_Reply_Char_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_Reply_Header_Type TTCN3_reply_header;

    public TTCN3_Reply_Header_Type get_TTCN3_reply_header() {
      this.lazyInit();
      return this.TTCN3_reply_header;
    }

    public void set_TTCN3_reply_header(Data TTCN3_reply_header) {
      this.lazyInit();
      this.TTCN3_reply_header.set(TTCN3_reply_header);
    }

    private TTCN3_Reply_Body_Char_Type TTCN3_reply_body;

    public TTCN3_Reply_Body_Char_Type get_TTCN3_reply_body() {
      this.lazyInit();
      return this.TTCN3_reply_body;
    }

    public void set_TTCN3_reply_body(Data TTCN3_reply_body) {
      this.lazyInit();
      this.TTCN3_reply_body.set(TTCN3_reply_body);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Reply_Char_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_reply_header = new TTCN3_Reply_Header_Type();
      this.addField("reply_header", TTCN3_reply_header);
      this.TTCN3_reply_body = new TTCN3_Reply_Body_Char_Type();
      this.addField("reply_body", TTCN3_reply_body);
    }

    public void setValue(TTCN3_GIOP_Reply_Char_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_reply_header(value.get_TTCN3_reply_header());
      this.set_TTCN3_reply_body(value.get_TTCN3_reply_body());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Reply_Char_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Reply_Char_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_reply_header().matches(value.get_TTCN3_reply_header())) {
        return false;
      }
      if (!get_TTCN3_reply_body().matches(value.get_TTCN3_reply_body())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Reply_Char_Type) {
        return this.matchesValue((TTCN3_GIOP_Reply_Char_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_Reply_String_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Reply_String_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Reply_String_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Reply_String_Type();
    }
  };

  public class TTCN3_GIOP_Reply_String_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_Reply_Header_Type TTCN3_reply_header;

    public TTCN3_Reply_Header_Type get_TTCN3_reply_header() {
      this.lazyInit();
      return this.TTCN3_reply_header;
    }

    public void set_TTCN3_reply_header(Data TTCN3_reply_header) {
      this.lazyInit();
      this.TTCN3_reply_header.set(TTCN3_reply_header);
    }

    private TTCN3_Reply_Body_String_Type TTCN3_reply_body;

    public TTCN3_Reply_Body_String_Type get_TTCN3_reply_body() {
      this.lazyInit();
      return this.TTCN3_reply_body;
    }

    public void set_TTCN3_reply_body(Data TTCN3_reply_body) {
      this.lazyInit();
      this.TTCN3_reply_body.set(TTCN3_reply_body);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Reply_String_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_reply_header = new TTCN3_Reply_Header_Type();
      this.addField("reply_header", TTCN3_reply_header);
      this.TTCN3_reply_body = new TTCN3_Reply_Body_String_Type();
      this.addField("reply_body", TTCN3_reply_body);
    }

    public void setValue(TTCN3_GIOP_Reply_String_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_reply_header(value.get_TTCN3_reply_header());
      this.set_TTCN3_reply_body(value.get_TTCN3_reply_body());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Reply_String_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Reply_String_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_reply_header().matches(value.get_TTCN3_reply_header())) {
        return false;
      }
      if (!get_TTCN3_reply_body().matches(value.get_TTCN3_reply_body())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Reply_String_Type) {
        return this.matchesValue((TTCN3_GIOP_Reply_String_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_Reply_Enum_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_Reply_Enum_Type",
                 Type.RECORD,
                 TTCN3_GIOP_Reply_Enum_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_Reply_Enum_Type();
    }
  };

  public class TTCN3_GIOP_Reply_Enum_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_Reply_Header_Type TTCN3_reply_header;

    public TTCN3_Reply_Header_Type get_TTCN3_reply_header() {
      this.lazyInit();
      return this.TTCN3_reply_header;
    }

    public void set_TTCN3_reply_header(Data TTCN3_reply_header) {
      this.lazyInit();
      this.TTCN3_reply_header.set(TTCN3_reply_header);
    }

    private TTCN3_Reply_Body_Enum_Type TTCN3_reply_body;

    public TTCN3_Reply_Body_Enum_Type get_TTCN3_reply_body() {
      this.lazyInit();
      return this.TTCN3_reply_body;
    }

    public void set_TTCN3_reply_body(Data TTCN3_reply_body) {
      this.lazyInit();
      this.TTCN3_reply_body.set(TTCN3_reply_body);
    }

    public Type getType() {
      return type_TTCN3_GIOP_Reply_Enum_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_reply_header = new TTCN3_Reply_Header_Type();
      this.addField("reply_header", TTCN3_reply_header);
      this.TTCN3_reply_body = new TTCN3_Reply_Body_Enum_Type();
      this.addField("reply_body", TTCN3_reply_body);
    }

    public void setValue(TTCN3_GIOP_Reply_Enum_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_reply_header(value.get_TTCN3_reply_header());
      this.set_TTCN3_reply_body(value.get_TTCN3_reply_body());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_Reply_Enum_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_Reply_Enum_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_reply_header().matches(value.get_TTCN3_reply_header())) {
        return false;
      }
      if (!get_TTCN3_reply_body().matches(value.get_TTCN3_reply_body())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_Reply_Enum_Type) {
        return this.matchesValue((TTCN3_GIOP_Reply_Enum_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_LocateRequest_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_LocateRequest_Type",
                 Type.RECORD,
                 TTCN3_GIOP_LocateRequest_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_LocateRequest_Type();
    }
  };

  public class TTCN3_GIOP_LocateRequest_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_LocateRequest_Header_Type TTCN3_locateRequest_header;

    public TTCN3_LocateRequest_Header_Type get_TTCN3_locateRequest_header() {
      this.lazyInit();
      return this.TTCN3_locateRequest_header;
    }

    public
      void set_TTCN3_locateRequest_header(Data TTCN3_locateRequest_header) {
      this.lazyInit();
      this.TTCN3_locateRequest_header.set(TTCN3_locateRequest_header);
    }

    public Type getType() {
      return type_TTCN3_GIOP_LocateRequest_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_locateRequest_header = new TTCN3_LocateRequest_Header_Type();
      this.addField("locateRequest_header", TTCN3_locateRequest_header);
    }

    public void setValue(TTCN3_GIOP_LocateRequest_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this
      .set_TTCN3_locateRequest_header(value.get_TTCN3_locateRequest_header());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_LocateRequest_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_LocateRequest_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_locateRequest_header()
           .matches(value.get_TTCN3_locateRequest_header())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_LocateRequest_Type) {
        return this.matchesValue((TTCN3_GIOP_LocateRequest_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_LocateReply_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_LocateReply_Type",
                 Type.RECORD,
                 TTCN3_GIOP_LocateReply_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_LocateReply_Type();
    }
  };

  public class TTCN3_GIOP_LocateReply_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    private TTCN3_LocateReply_Header_Type TTCN3_locateReply_header;

    public TTCN3_LocateReply_Header_Type get_TTCN3_locateReply_header() {
      this.lazyInit();
      return this.TTCN3_locateReply_header;
    }

    public void set_TTCN3_locateReply_header(Data TTCN3_locateReply_header) {
      this.lazyInit();
      this.TTCN3_locateReply_header.set(TTCN3_locateReply_header);
    }

    public Type getType() {
      return type_TTCN3_GIOP_LocateReply_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
      this.TTCN3_locateReply_header = new TTCN3_LocateReply_Header_Type();
      this.addField("locateReply_header", TTCN3_locateReply_header);
    }

    public void setValue(TTCN3_GIOP_LocateReply_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
      this.set_TTCN3_locateReply_header(value.get_TTCN3_locateReply_header());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_LocateReply_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_LocateReply_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      if (!get_TTCN3_locateReply_header()
           .matches(value.get_TTCN3_locateReply_header())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_LocateReply_Type) {
        return this.matchesValue((TTCN3_GIOP_LocateReply_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  public Type type_TTCN3_GIOP_MessageError_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "GIOP_MessageError_Type",
                 Type.RECORD,
                 TTCN3_GIOP_MessageError_Type.class) {
    public Value newInstance() {
      return new TTCN3_GIOP_MessageError_Type();
    }
  };

  public class TTCN3_GIOP_MessageError_Type extends Record {
    private TTCN3_GIOP_Header_Type TTCN3_giop_header;

    public TTCN3_GIOP_Header_Type get_TTCN3_giop_header() {
      this.lazyInit();
      return this.TTCN3_giop_header;
    }

    public void set_TTCN3_giop_header(Data TTCN3_giop_header) {
      this.lazyInit();
      this.TTCN3_giop_header.set(TTCN3_giop_header);
    }

    public Type getType() {
      return type_TTCN3_GIOP_MessageError_Type;
    }

    protected void init() {
      super.init();
      this.TTCN3_giop_header = new TTCN3_GIOP_Header_Type();
      this.addField("giop_header", TTCN3_giop_header);
    }

    public void setValue(TTCN3_GIOP_MessageError_Type value) {
      this.set_TTCN3_giop_header(value.get_TTCN3_giop_header());
    }

    public void setValue(Data value) {
      this.setValue(((TTCN3_GIOP_MessageError_Type)value));
    }

    public boolean matchesValue(TTCN3_GIOP_MessageError_Type value) {
      if (!get_TTCN3_giop_header().matches(value.get_TTCN3_giop_header())) {
        return false;
      }
      return true;
    }

    public boolean matchesValue(Data value) {
      if (this instanceof TTCN3_GIOP_MessageError_Type) {
        return this.matchesValue((TTCN3_GIOP_MessageError_Type)value);
      } else {
        return super.matchesValue(value);
      }
    }
  }

  abstract class Type_7 extends Function {
    public abstract TTCN3_GIOP_Header_Type apply(Data arg0, Data arg1);

    public TTCN3_GIOP_Header_Type apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_8 extends Function {
    public abstract TTCN3_GIOP_Header_Type apply(Data arg0, Data arg1);

    public TTCN3_GIOP_Header_Type apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_9 extends Function {
    public abstract TTCN3_GIOP_Header_Type apply(Data arg0, Data arg1);

    public TTCN3_GIOP_Header_Type apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_10 extends Function {
    public abstract TTCN3_GIOP_Header_Type apply(Data arg0, Data arg1);

    public TTCN3_GIOP_Header_Type apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_11 extends Function {
    public abstract TTCN3_GIOP_Header_Type apply(Data arg0, Data arg1);

    public TTCN3_GIOP_Header_Type apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_12 extends Function {
    public abstract TTCN3_Request_Header_Type apply(Data arg0, Data arg1);

    public TTCN3_Request_Header_Type apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_13 extends Function {
    public abstract TTCN3_Reply_Header_Type apply(Data arg0, Data arg1);

    public TTCN3_Reply_Header_Type apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_14 extends Function {
    public abstract TTCN3_LocateRequest_Header_Type apply(Data arg0);

    public TTCN3_LocateRequest_Header_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_15 extends Function {
    public abstract TTCN3_LocateReply_Header_Type apply(Data arg0);

    public TTCN3_LocateReply_Header_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_16 extends Function {
    public abstract TTCN3_LocateRequest_Header_Type apply(Data arg0);

    public TTCN3_LocateRequest_Header_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_17 extends Function {
    public abstract TTCN3_LocateReply_Header_Type apply(Data arg0);

    public TTCN3_LocateReply_Header_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  public final Type_7 TTCN3_GIOP_Header =
  new Type_7() {
    public TTCN3_GIOP_Header_Type apply(Data arg0, Data arg1) {
      final OctetString TTCN3_Msg_Type =
        (OctetString)new OctetString().init(arg0);
      final TTCN3_CORBA_ULong TTCN3_size =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg1);
      return
        new TTCN3_GIOP_Header_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_magic(new Type_3() {
                               protected void init() {
                                 super.init();
                                 this
                                 .set(((IntegerList)new CharString("GIOP")));
                                 this.toValue();
                               }
                             });
              this
            .set_TTCN3_version(new Type_4() {
                                 protected void init() {
                                   super.init();
                                   this.setLength(2);
                                   this.setItem(0, new Octet(1));
                                   this.setItem(1, new Octet(2));
                                   this.toValue();
                                 }
                               });
              this
            .set_TTCN3_byteorder(new TTCN3_CORBA_Octet() {
                                   protected void init() {
                                     super.init();
                                     this.setLength(1);
                                     this.setItem(0, new Octet(0));
                                     this.toValue();
                                   }
                                 });
            this.set_TTCN3_messagetype(TTCN3_Msg_Type);
            this.set_TTCN3_messagesize(TTCN3_size);
            this.toValue();
          }
        };
    }
  };

  public final Type_8 TTCN3_WrongMagic_GIOP_Header =
  new Type_8() {
    public TTCN3_GIOP_Header_Type apply(Data arg0, Data arg1) {
      final OctetString TTCN3_Msg_Type =
        (OctetString)new OctetString().init(arg0);
      final TTCN3_CORBA_ULong TTCN3_size =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg1);
      return
        new TTCN3_GIOP_Header_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_magic(new Type_3() {
                               protected void init() {
                                 super.init();
                                 this
                                 .set(((IntegerList)new CharString("GIOR")));
                                 this.toValue();
                               }
                             });
              this
            .set_TTCN3_version(new Type_4() {
                                 protected void init() {
                                   super.init();
                                   this.setLength(2);
                                   this.setItem(0, new Octet(1));
                                   this.setItem(1, new Octet(2));
                                   this.toValue();
                                 }
                               });
              this
            .set_TTCN3_byteorder(new TTCN3_CORBA_Octet() {
                                   protected void init() {
                                     super.init();
                                     this.setLength(1);
                                     this.setItem(0, new Octet(0));
                                     this.toValue();
                                   }
                                 });
            this.set_TTCN3_messagetype(TTCN3_Msg_Type);
            this.set_TTCN3_messagesize(TTCN3_size);
            this.toValue();
          }
        };
    }
  };

  public final Type_9 TTCN3_WrongVersion_GIOP_Header =
  new Type_9() {
    public TTCN3_GIOP_Header_Type apply(Data arg0, Data arg1) {
      final OctetString TTCN3_Msg_Type =
        (OctetString)new OctetString().init(arg0);
      final TTCN3_CORBA_ULong TTCN3_size =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg1);
      return
        new TTCN3_GIOP_Header_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_magic(new Type_3() {
                               protected void init() {
                                 super.init();
                                 this
                                 .set(((IntegerList)new CharString("GIOP")));
                                 this.toValue();
                               }
                             });
              this
            .set_TTCN3_version(new Type_4() {
                                 protected void init() {
                                   super.init();
                                   this.setLength(2);
                                   this.setItem(0, new Octet(9));
                                   this.setItem(1, new Octet(9));
                                   this.toValue();
                                 }
                               });
              this
            .set_TTCN3_byteorder(new TTCN3_CORBA_Octet() {
                                   protected void init() {
                                     super.init();
                                     this.setLength(1);
                                     this.setItem(0, new Octet(0));
                                     this.toValue();
                                   }
                                 });
            this.set_TTCN3_messagetype(TTCN3_Msg_Type);
            this.set_TTCN3_messagesize(TTCN3_size);
            this.toValue();
          }
        };
    }
  };

  public final Type_10 TTCN3_GIOP_Header_1_0 =
  new Type_10() {
    public TTCN3_GIOP_Header_Type apply(Data arg0, Data arg1) {
      final OctetString TTCN3_Msg_Type =
        (OctetString)new OctetString().init(arg0);
      final TTCN3_CORBA_ULong TTCN3_size =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg1);
      return
        new TTCN3_GIOP_Header_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_magic(new Type_3() {
                               protected void init() {
                                 super.init();
                                 this
                                 .set(((IntegerList)new CharString("GIOP")));
                                 this.toValue();
                               }
                             });
              this
            .set_TTCN3_version(new Type_4() {
                                 protected void init() {
                                   super.init();
                                   this.setLength(2);
                                   this.setItem(0, new Octet(1));
                                   this.setItem(1, new Octet(0));
                                   this.toValue();
                                 }
                               });
              this
            .set_TTCN3_byteorder(new TTCN3_CORBA_Octet() {
                                   protected void init() {
                                     super.init();
                                     this.setLength(1);
                                     this.setItem(0, new Octet(0));
                                     this.toValue();
                                   }
                                 });
            this.set_TTCN3_messagetype(TTCN3_Msg_Type);
            this.set_TTCN3_messagesize(TTCN3_size);
            this.toValue();
          }
        };
    }
  };

  public final Type_11 TTCN3_GIOP_Header_1_1 =
  new Type_11() {
    public TTCN3_GIOP_Header_Type apply(Data arg0, Data arg1) {
      final OctetString TTCN3_Msg_Type =
        (OctetString)new OctetString().init(arg0);
      final TTCN3_CORBA_ULong TTCN3_size =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg1);
      return
        new TTCN3_GIOP_Header_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_magic(new Type_3() {
                               protected void init() {
                                 super.init();
                                 this
                                 .set(((IntegerList)new CharString("GIOP")));
                                 this.toValue();
                               }
                             });
              this
            .set_TTCN3_version(new Type_4() {
                                 protected void init() {
                                   super.init();
                                   this.setLength(2);
                                   this.setItem(0, new Octet(1));
                                   this.setItem(1, new Octet(1));
                                   this.toValue();
                                 }
                               });
              this
            .set_TTCN3_byteorder(new TTCN3_CORBA_Octet() {
                                   protected void init() {
                                     super.init();
                                     this.setLength(1);
                                     this.setItem(0, new Octet(0));
                                     this.toValue();
                                   }
                                 });
            this.set_TTCN3_messagetype(TTCN3_Msg_Type);
            this.set_TTCN3_messagesize(TTCN3_size);
            this.toValue();
          }
        };
    }
  };

  public final TTCN3_TargetAddress_Type TTCN3_TargetAddress =
  new TTCN3_TargetAddress_Type() {
    protected void init() {
      super.init();
      this.set_TTCN3_object_key(TTCN3_get_object_key.apply());
      this.toValue();
    }
  };

  public final TTCN3_TargetAddress_Type TTCN3_TargetAddress_unknown =
  new TTCN3_TargetAddress_Type() {
    protected void init() {
      super.init();
        this
      .set_TTCN3_object_key(new OctetString() {
                              protected void init() {
                                super.init();
                                this.setLength(6);
                                this.setItem(0, new Octet(0));
                                this.setItem(1, new Octet(0));
                                this.setItem(2, new Octet(0));
                                this.setItem(3, new Octet(2));
                                this.setItem(4, new Octet(71));
                                this.setItem(5, new Octet(17));
                                this.toValue();
                              }
                            });
      this.toValue();
    }
  };

  public final Type_12 TTCN3_Request_Header =
  new Type_12() {
    public TTCN3_Request_Header_Type apply(Data arg0, Data arg1) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      final CharString TTCN3_op = (CharString)new CharString().init(arg1);
      return
        new TTCN3_Request_Header_Type() {
          protected void init() {
            super.init();
            this.set_TTCN3_request_id(TTCN3_req_id);
              this
            .set_TTCN3_response_flags(new TTCN3_CORBA_Octet() {
                                        protected void init() {
                                          super.init();
                                          this.setLength(1);
                                          this.setItem(0, new Octet(3));
                                          this.toValue();
                                        }
                                      });
              this
            .set_TTCN3_reserved(new Type_5() {
                                  protected void init() {
                                    super.init();
                                    this.setLength(3);
                                    this.setItem(0, new Octet(0));
                                    this.setItem(1, new Octet(0));
                                    this.setItem(2, new Octet(0));
                                    this.toValue();
                                  }
                                });
            this.set_TTCN3_addressing_disposition(TTCN3_KeyAddr);
            this.set_TTCN3_target(TTCN3_TargetAddress);
            this.set_TTCN3_operation(TTCN3_op);
              this
            .set_TTCN3_service_context((TTCN3_CORBA_ULong)new
                                                            TTCN3_CORBA_ULong()
                                                          .init(new Int(0)));
            this.toValue();
          }
        };
    }
  };

  public final Type_13 TTCN3_Reply_Header =
  new Type_13() {
    public TTCN3_Reply_Header_Type apply(Data arg0, Data arg1) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      final TTCN3_CORBA_ULong TTCN3_rep_stat =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg1);
      return
        new TTCN3_Reply_Header_Type() {
          protected void init() {
            super.init();
            this.set_TTCN3_request_id(TTCN3_req_id);
            this.set_TTCN3_reply_status(TTCN3_rep_stat);
              this
            .set_TTCN3_service_context((TTCN3_CORBA_ULong)new
                                                            TTCN3_CORBA_ULong()
                                                          .init(new Int(0)));
            this.toValue();
          }
        };
    }
  };

  public final TTCN3_Reply_Body_Boolean_Type TTCN3_Reply_Boolean =
  new TTCN3_Reply_Body_Boolean_Type() {
    protected void init() {
      super.init();
      this.setLength(1);
      this.setItem(0, new Octet(1));
      this.toValue();
    }
  };

  public final TTCN3_Reply_Body_Long_Type TTCN3_Reply_Long =
  (TTCN3_Reply_Body_Long_Type)new TTCN3_Reply_Body_Long_Type()
                              .init(new Int(2147483647));

  public final TTCN3_Reply_Body_LongLong_Type TTCN3_Reply_LongLong =
  (TTCN3_Reply_Body_LongLong_Type)new TTCN3_Reply_Body_LongLong_Type()
                                  .init(new Int(92233));

  public final TTCN3_Reply_Body_Short_Type TTCN3_Reply_Short =
  (TTCN3_Reply_Body_Short_Type)new TTCN3_Reply_Body_Short_Type()
                               .init(new Int(32767));

  public final TTCN3_Reply_Body_Octet_Type TTCN3_Reply_Octet =
  new TTCN3_Reply_Body_Octet_Type() {
    protected void init() {
      super.init();
      this.setLength(1);
      this.setItem(0, new Octet(8));
      this.toValue();
    }
  };

  public final TTCN3_Reply_Body_Float_Type TTCN3_Reply_Float =
  (TTCN3_Reply_Body_Float_Type)new TTCN3_Reply_Body_Float_Type()
                               .init(new Float(3.4E38F));

  public final TTCN3_Reply_Body_Double_Type TTCN3_Reply_Double =
  (TTCN3_Reply_Body_Double_Type)new TTCN3_Reply_Body_Double_Type()
                                .init(new Float(3.14F));

  public final TTCN3_Reply_Body_Char_Type TTCN3_Reply_Char =
  new TTCN3_Reply_Body_Char_Type() {
    protected void init() {
      super.init();
      this.set(((IntegerList)new CharString("$")));
      this.toValue();
    }
  };

  public final TTCN3_Reply_Body_String_Type TTCN3_Reply_String =
  new TTCN3_Reply_Body_String_Type() {
    protected void init() {
      super.init();
      this.set(((IntegerList)new CharString("hello!")));
      this.toValue();
    }
  };

  public final TTCN3_Reply_Body_Enum_Type TTCN3_Reply_Enum =
  (TTCN3_Reply_Body_Enum_Type)new TTCN3_Reply_Body_Enum_Type()
                              .init(new Int(0));

  public final Type_14 TTCN3_LocateRequest_Header =
  new Type_14() {
    public TTCN3_LocateRequest_Header_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_LocateRequest_Header_Type() {
          protected void init() {
            super.init();
            this.set_TTCN3_request_id(TTCN3_req_id);
            this.set_TTCN3_addressing_disposition(TTCN3_KeyAddr);
            this.set_TTCN3_target(TTCN3_TargetAddress);
            this.toValue();
          }
        };
    }
  };

  public final Type_15 TTCN3_LocateReply_Header =
  new Type_15() {
    public TTCN3_LocateReply_Header_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_LocateReply_Header_Type() {
          protected void init() {
            super.init();
            this.set_TTCN3_request_id(TTCN3_req_id);
            this.set_TTCN3_locate_status(TTCN3_OBJECT_HERE);
            this.toValue();
          }
        };
    }
  };

  public final Type_16 TTCN3_LocateRequest_Header_unknown =
  new Type_16() {
    public TTCN3_LocateRequest_Header_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_LocateRequest_Header_Type() {
          protected void init() {
            super.init();
            this.set_TTCN3_request_id(TTCN3_req_id);
            this.set_TTCN3_addressing_disposition(TTCN3_KeyAddr);
            this.set_TTCN3_target(TTCN3_TargetAddress_unknown);
            this.toValue();
          }
        };
    }
  };

  public final Type_17 TTCN3_LocateReply_Header_unknown =
  new Type_17() {
    public TTCN3_LocateReply_Header_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_LocateReply_Header_Type() {
          protected void init() {
            super.init();
            this.set_TTCN3_request_id(TTCN3_req_id);
            this.set_TTCN3_locate_status(TTCN3_UNKNOWN_OBJECT);
            this.toValue();
          }
        };
    }
  };

  abstract class Type_18 extends Function {
    public abstract TTCN3_GIOP_Request_Type apply(Data arg0, Data arg1);

    public TTCN3_GIOP_Request_Type apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_19 extends Function {
    public abstract TTCN3_GIOP_Reply_Boolean_Type apply(Data arg0);

    public TTCN3_GIOP_Reply_Boolean_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_20 extends Function {
    public abstract TTCN3_GIOP_Reply_Long_Type apply(Data arg0);

    public TTCN3_GIOP_Reply_Long_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_21 extends Function {
    public abstract TTCN3_GIOP_Reply_LongLong_Type apply(Data arg0);

    public TTCN3_GIOP_Reply_LongLong_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_22 extends Function {
    public abstract TTCN3_GIOP_Reply_Short_Type apply(Data arg0);

    public TTCN3_GIOP_Reply_Short_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_23 extends Function {
    public abstract TTCN3_GIOP_Reply_Octet_Type apply(Data arg0);

    public TTCN3_GIOP_Reply_Octet_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_24 extends Function {
    public abstract TTCN3_GIOP_Reply_Float_Type apply(Data arg0);

    public TTCN3_GIOP_Reply_Float_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_25 extends Function {
    public abstract TTCN3_GIOP_Reply_Double_Type apply(Data arg0);

    public TTCN3_GIOP_Reply_Double_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_26 extends Function {
    public abstract TTCN3_GIOP_Reply_Char_Type apply(Data arg0);

    public TTCN3_GIOP_Reply_Char_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_27 extends Function {
    public abstract TTCN3_GIOP_Reply_String_Type apply(Data arg0);

    public TTCN3_GIOP_Reply_String_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_28 extends Function {
    public abstract TTCN3_GIOP_Reply_Enum_Type apply(Data arg0);

    public TTCN3_GIOP_Reply_Enum_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_29 extends Function {
    public abstract TTCN3_GIOP_LocateRequest_Type apply(Data arg0);

    public TTCN3_GIOP_LocateRequest_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_30 extends Function {
    public abstract TTCN3_GIOP_LocateReply_Type apply(Data arg0);

    public TTCN3_GIOP_LocateReply_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_31 extends Function {
    public abstract TTCN3_GIOP_LocateRequest_Type apply(Data arg0);

    public TTCN3_GIOP_LocateRequest_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_32 extends Function {
    public abstract TTCN3_GIOP_LocateReply_Type apply(Data arg0);

    public TTCN3_GIOP_LocateReply_Type apply(Data[] args) {
      return this.apply(args[0]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_33 extends Function {
    public abstract TTCN3_GIOP_Request_Type apply(Data arg0, Data arg1);

    public TTCN3_GIOP_Request_Type apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_34 extends Function {
    public abstract TTCN3_GIOP_Request_Type apply(Data arg0, Data arg1);

    public TTCN3_GIOP_Request_Type apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_35 extends Function {
    public abstract TTCN3_GIOP_Request_Type apply(Data arg0, Data arg1);

    public TTCN3_GIOP_Request_Type apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  public final Type_18 TTCN3_request =
  new Type_18() {
    public TTCN3_GIOP_Request_Type apply(Data arg0, Data arg1) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      final CharString TTCN3_op = (CharString)new CharString().init(arg1);
      return
        new TTCN3_GIOP_Request_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_REQUEST,
                                          (TTCN3_CORBA_ULong)
                                          new TTCN3_CORBA_ULong()
                                          .init(new Int(0))));
              this
            .set_TTCN3_request_header(TTCN3_Request_Header
                                      .apply(TTCN3_req_id, TTCN3_op));
            this.toValue();
          }
        };
    }
  };

  public final Type_19 TTCN3_boolean_reply =
  new Type_19() {
    public TTCN3_GIOP_Reply_Boolean_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_Reply_Boolean_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_REPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_reply_header(TTCN3_Reply_Header
                                    .apply(TTCN3_req_id, TTCN3_NO_EXCEPTION));
            this.set_TTCN3_reply_body(TTCN3_Reply_Boolean);
            this.toValue();
          }
        };
    }
  };

  public final Type_20 TTCN3_long_reply =
  new Type_20() {
    public TTCN3_GIOP_Reply_Long_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_Reply_Long_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_REPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_reply_header(TTCN3_Reply_Header
                                    .apply(TTCN3_req_id, TTCN3_NO_EXCEPTION));
            this.set_TTCN3_reply_body(TTCN3_Reply_Long);
            this.toValue();
          }
        };
    }
  };

  public final Type_21 TTCN3_longlong_reply =
  new Type_21() {
    public TTCN3_GIOP_Reply_LongLong_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_Reply_LongLong_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_REPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_reply_header(TTCN3_Reply_Header
                                    .apply(TTCN3_req_id, TTCN3_NO_EXCEPTION));
            this.set_TTCN3_reply_body(TTCN3_Reply_LongLong);
            this.toValue();
          }
        };
    }
  };

  public final Type_22 TTCN3_short_reply =
  new Type_22() {
    public TTCN3_GIOP_Reply_Short_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_Reply_Short_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_REPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_reply_header(TTCN3_Reply_Header
                                    .apply(TTCN3_req_id, TTCN3_NO_EXCEPTION));
            this.set_TTCN3_reply_body(TTCN3_Reply_Short);
            this.toValue();
          }
        };
    }
  };

  public final Type_23 TTCN3_octet_reply =
  new Type_23() {
    public TTCN3_GIOP_Reply_Octet_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_Reply_Octet_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_REPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_reply_header(TTCN3_Reply_Header
                                    .apply(TTCN3_req_id, TTCN3_NO_EXCEPTION));
            this.set_TTCN3_reply_body(TTCN3_Reply_Octet);
            this.toValue();
          }
        };
    }
  };

  public final Type_24 TTCN3_float_reply =
  new Type_24() {
    public TTCN3_GIOP_Reply_Float_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_Reply_Float_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_REPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_reply_header(TTCN3_Reply_Header
                                    .apply(TTCN3_req_id, TTCN3_NO_EXCEPTION));
            this.set_TTCN3_reply_body(TTCN3_Reply_Float);
            this.toValue();
          }
        };
    }
  };

  public final Type_25 TTCN3_double_reply =
  new Type_25() {
    public TTCN3_GIOP_Reply_Double_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_Reply_Double_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_REPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_reply_header(TTCN3_Reply_Header
                                    .apply(TTCN3_req_id, TTCN3_NO_EXCEPTION));
            this.set_TTCN3_reply_body(TTCN3_Reply_Double);
            this.toValue();
          }
        };
    }
  };

  public final Type_26 TTCN3_char_reply =
  new Type_26() {
    public TTCN3_GIOP_Reply_Char_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_Reply_Char_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_REPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_reply_header(TTCN3_Reply_Header
                                    .apply(TTCN3_req_id, TTCN3_NO_EXCEPTION));
            this.set_TTCN3_reply_body(TTCN3_Reply_Char);
            this.toValue();
          }
        };
    }
  };

  public final Type_27 TTCN3_string_reply =
  new Type_27() {
    public TTCN3_GIOP_Reply_String_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_Reply_String_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_REPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_reply_header(TTCN3_Reply_Header
                                    .apply(TTCN3_req_id, TTCN3_NO_EXCEPTION));
            this.set_TTCN3_reply_body(TTCN3_Reply_String);
            this.toValue();
          }
        };
    }
  };

  public final Type_28 TTCN3_enum_reply =
  new Type_28() {
    public TTCN3_GIOP_Reply_Enum_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_Reply_Enum_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_REPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_reply_header(TTCN3_Reply_Header
                                    .apply(TTCN3_req_id, TTCN3_NO_EXCEPTION));
            this.set_TTCN3_reply_body(TTCN3_Reply_Enum);
            this.toValue();
          }
        };
    }
  };

  public final Type_29 TTCN3_locateRequest =
  new Type_29() {
    public TTCN3_GIOP_LocateRequest_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_LocateRequest_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_LOCATEREQUEST,
                                          (TTCN3_CORBA_ULong)
                                          new TTCN3_CORBA_ULong()
                                          .init(new Int(0))));
              this
            .set_TTCN3_locateRequest_header(TTCN3_LocateRequest_Header
                                            .apply(TTCN3_req_id));
            this.toValue();
          }
        };
    }
  };

  public final Type_30 TTCN3_locateReply =
  new Type_30() {
    public TTCN3_GIOP_LocateReply_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_LocateReply_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_LOCATEREPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_locateReply_header(TTCN3_LocateReply_Header
                                          .apply(TTCN3_req_id));
            this.toValue();
          }
        };
    }
  };

  public final Type_31 TTCN3_locateRequest_unknown =
  new Type_31() {
    public TTCN3_GIOP_LocateRequest_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_LocateRequest_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_LOCATEREQUEST,
                                          (TTCN3_CORBA_ULong)
                                          new TTCN3_CORBA_ULong()
                                          .init(new Int(0))));
              this
            .set_TTCN3_locateRequest_header(TTCN3_LocateRequest_Header_unknown
                                            .apply(TTCN3_req_id));
            this.toValue();
          }
        };
    }
  };

  public final Type_32 TTCN3_locateReply_unknown =
  new Type_32() {
    public TTCN3_GIOP_LocateReply_Type apply(Data arg0) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      return
        new TTCN3_GIOP_LocateReply_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_LOCATEREPLY,
                                          ((TTCN3_CORBA_ULong)
                                           new TTCN3_CORBA_ULong()
                                           .toPresent())));
              this
            .set_TTCN3_locateReply_header(TTCN3_LocateReply_Header_unknown
                                          .apply(TTCN3_req_id));
            this.toValue();
          }
        };
    }
  };

  public final Type_33 TTCN3_wrong_magic =
  new Type_33() {
    public TTCN3_GIOP_Request_Type apply(Data arg0, Data arg1) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      final CharString TTCN3_op = (CharString)new CharString().init(arg1);
      return
        new TTCN3_GIOP_Request_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_WrongMagic_GIOP_Header
                                   .apply(TTCN3_REQUEST,
                                          (TTCN3_CORBA_ULong)
                                          new TTCN3_CORBA_ULong()
                                          .init(new Int(0))));
              this
            .set_TTCN3_request_header(TTCN3_Request_Header
                                      .apply(TTCN3_req_id, TTCN3_op));
            this.toValue();
          }
        };
    }
  };

  public final Type_34 TTCN3_wrong_version =
  new Type_34() {
    public TTCN3_GIOP_Request_Type apply(Data arg0, Data arg1) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      final CharString TTCN3_op = (CharString)new CharString().init(arg1);
      return
        new TTCN3_GIOP_Request_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_WrongVersion_GIOP_Header
                                   .apply(TTCN3_REQUEST,
                                          (TTCN3_CORBA_ULong)
                                          new TTCN3_CORBA_ULong()
                                          .init(new Int(0))));
              this
            .set_TTCN3_request_header(TTCN3_Request_Header
                                      .apply(TTCN3_req_id, TTCN3_op));
            this.toValue();
          }
        };
    }
  };

  public final Type_35 TTCN3_wrong_messagetype =
  new Type_35() {
    public TTCN3_GIOP_Request_Type apply(Data arg0, Data arg1) {
      final TTCN3_CORBA_ULong TTCN3_req_id =
        (TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong().init(arg0);
      final CharString TTCN3_op = (CharString)new CharString().init(arg1);
      return
        new TTCN3_GIOP_Request_Type() {
          protected void init() {
            super.init();
              this
            .set_TTCN3_giop_header(TTCN3_GIOP_Header
                                   .apply(TTCN3_WRONGMESSAGETYPE,
                                          (TTCN3_CORBA_ULong)
                                          new TTCN3_CORBA_ULong()
                                          .init(new Int(0))));
              this
            .set_TTCN3_request_header(TTCN3_Request_Header
                                      .apply(TTCN3_req_id, TTCN3_op));
            this.toValue();
          }
        };
    }
  };

  public final TTCN3_GIOP_MessageError_Type TTCN3_message_error_1_0 =
  new TTCN3_GIOP_MessageError_Type() {
    protected void init() {
      super.init();
        this
      .set_TTCN3_giop_header(TTCN3_GIOP_Header_1_0
                             .apply(TTCN3_MESSAGEERROR,
                                    ((TTCN3_CORBA_ULong)
                                     new TTCN3_CORBA_ULong().toPresent())));
      this.toValue();
    }
  };

  public final TTCN3_GIOP_MessageError_Type TTCN3_message_error_1_1 =
  new TTCN3_GIOP_MessageError_Type() {
    protected void init() {
      super.init();
        this
      .set_TTCN3_giop_header(TTCN3_GIOP_Header_1_1
                             .apply(TTCN3_MESSAGEERROR,
                                    ((TTCN3_CORBA_ULong)
                                     new TTCN3_CORBA_ULong().toPresent())));
      this.toValue();
    }
  };

  public final TTCN3_GIOP_MessageError_Type TTCN3_message_error_1_2 =
  new TTCN3_GIOP_MessageError_Type() {
    protected void init() {
      super.init();
        this
      .set_TTCN3_giop_header(TTCN3_GIOP_Header
                             .apply(TTCN3_MESSAGEERROR,
                                    ((TTCN3_CORBA_ULong)
                                     new TTCN3_CORBA_ULong().toPresent())));
      this.toValue();
    }
  };

  public class TTCN3_giop_port_type extends PortReference {
  }

  public Type type_Interface_TTCN3_MTC_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "Interface_TTCN3_MTC_Type",
                 Type.COMPONENT,
                 Interface_TTCN3_MTC_Type.class) {
    public Value newInstance() {
      return new Interface_TTCN3_MTC_Type();
    }
  };

  public class Interface_TTCN3_MTC_Type extends ComponentInterface {
    public Type getType() {
      return type_Interface_TTCN3_MTC_Type;
    }

    private PortInterface TTCN3_giop_port;

    public PortInterface get_TTCN3_giop_port() {
      lazyInit();
      return TTCN3_giop_port;
    }

    private TimerReference TTCN3_Timer_reply;

    public TimerReference get_TTCN3_Timer_reply() {
      lazyInit();
      return TTCN3_Timer_reply;
    }

    protected void init() {
      super.init();
      TTCN3_giop_port = new PortInterface();
      TTCN3_giop_port.setName("giop_port");
      accessPort(TTCN3_giop_port);
      TTCN3_Timer_reply = new TimerReference();
      accessTimer("Timer_reply", TTCN3_Timer_reply);
      this.toValue();
    }
  }

  public Type type_TTCN3_MTC_Type =
  new
    AbstractType(TTCN3_GIOP_client,
                 "MTC_Type",
                 Type.COMPONENT,
                 TTCN3_MTC_Type.class) {
    public Value newInstance() {
      return new TTCN3_MTC_Type();
    }
  };

  public class TTCN3_MTC_Type extends ComponentReference {
    private TTCN3_giop_port_type TTCN3_giop_port;

    public TTCN3_giop_port_type get_TTCN3_giop_port() {
      lazyInit();
      return TTCN3_giop_port;
    }

    protected void init() {
      super.init();
      TTCN3_giop_port = new TTCN3_giop_port_type();
      TTCN3_giop_port.setName("giop_port");
      accessPort(TTCN3_giop_port);
      this.toValue();
    }

    public TTCN3_MTC_Type create() {
      createEntity();
      return this;
    }

    public Type getType() {
      return type_TTCN3_MTC_Type;
    }

    public Type getInterfaceType() {
      return new Interface_TTCN3_MTC_Type().getType();
    }
  }

  abstract class Type_37 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_36 extends Function {
    public abstract Type_37 apply(Data arg0, Data arg1);

    public Type_37 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_38 extends Function {
    public abstract TestCaseBehavior apply();

    public TestCaseBehavior apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_40 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_39 extends Function {
    public abstract Type_40 apply();

    public Type_40 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_42 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_41 extends Function {
    public abstract Type_42 apply(Data arg0, Data arg1);

    public Type_42 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_44 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_43 extends Function {
    public abstract Type_44 apply();

    public Type_44 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_46 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_45 extends Function {
    public abstract Type_46 apply(Data arg0, Data arg1);

    public Type_46 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_48 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_47 extends Function {
    public abstract Type_48 apply();

    public Type_48 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_50 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_49 extends Function {
    public abstract Type_50 apply(Data arg0, Data arg1);

    public Type_50 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_52 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_51 extends Function {
    public abstract Type_52 apply();

    public Type_52 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_54 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_53 extends Function {
    public abstract Type_54 apply(Data arg0, Data arg1);

    public Type_54 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_56 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_55 extends Function {
    public abstract Type_56 apply();

    public Type_56 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_58 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_57 extends Function {
    public abstract Type_58 apply(Data arg0, Data arg1);

    public Type_58 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_60 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_59 extends Function {
    public abstract Type_60 apply();

    public Type_60 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_62 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_61 extends Function {
    public abstract Type_62 apply(Data arg0, Data arg1);

    public Type_62 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_64 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_63 extends Function {
    public abstract Type_64 apply();

    public Type_64 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_66 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_65 extends Function {
    public abstract Type_66 apply(Data arg0, Data arg1);

    public Type_66 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_68 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_67 extends Function {
    public abstract Type_68 apply();

    public Type_68 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_70 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_69 extends Function {
    public abstract Type_70 apply(Data arg0, Data arg1);

    public Type_70 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_72 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_71 extends Function {
    public abstract Type_72 apply();

    public Type_72 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_74 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_73 extends Function {
    public abstract Type_74 apply(Data arg0, Data arg1);

    public Type_74 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_76 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_75 extends Function {
    public abstract Type_76 apply();

    public Type_76 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_78 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_77 extends Function {
    public abstract Type_78 apply(Data arg0, Data arg1);

    public Type_78 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_80 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_79 extends Function {
    public abstract Type_80 apply();

    public Type_80 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_82 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_81 extends Function {
    public abstract Type_82 apply(Data arg0, Data arg1);

    public Type_82 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_84 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_83 extends Function {
    public abstract Type_84 apply();

    public Type_84 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_86 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_85 extends Function {
    public abstract Type_86 apply(Data arg0, Data arg1);

    public Type_86 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_88 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_87 extends Function {
    public abstract Type_88 apply();

    public Type_88 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_90 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_89 extends Function {
    public abstract Type_90 apply(Data arg0, Data arg1);

    public Type_90 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_92 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_91 extends Function {
    public abstract Type_92 apply();

    public Type_92 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_94 extends Behavior {
    public abstract void on(Interface_TTCN3_MTC_Type self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        this.on((Interface_TTCN3_MTC_Type)self);
        return Void.Nothing;
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_93 extends Function {
    public abstract Type_94 apply(Data arg0, Data arg1);

    public Type_94 apply(Data[] args) {
      return this.apply(args[0], args[1]);
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  abstract class Type_96 extends Behavior {
    public abstract TTCN3_MTC_Type on(ComponentInterface self);
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  }

  abstract class Type_95 extends Function {
    public abstract Type_96 apply();

    public Type_96 apply(Data[] args) {
      return this.apply();
    }

    public Data applyFunction(Data[] args) {
      return this.apply(args);
    }
  }

  public final Type_1 TTCN3_get_object_key =
  new Type_1() {
    public OctetString apply() {
      return
        (OctetString)
        externalFunction("get_object_key",
                         new ExternalCallParameterList(new Data[] {}) {
                           public boolean isIn(int index) {
                             return true;
                           }
                           public boolean isOut(int index) {
                             return false;
                           }
                         },
                         new OctetString());
    }
  };
  public final CharString TTCN3_giopmagic = new CharString("GIOP");
  public final TTCN3_CORBA_Short TTCN3_KeyAddr =
  (TTCN3_CORBA_Short)new TTCN3_CORBA_Short().init(new Int(0));
  public final Type_36 body_of_Return_boolean =
  new Type_36() {
    public Type_37 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_37() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 486, 9, 486, 31); }
                if (DEBUG) { tracePoint(sourceFile, 488, 9, 488, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 489, 9, 489, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(1)));
                if (DEBUG) { tracePoint(sourceFile, 490, 9, 490, 58); }
                TTCN3_giop_port
                .send(TTCN3_request
                      .apply(TTCN3_req_id, new CharString("return_boolean")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 491, 9, 491, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 492, 9, 508, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_boolean_reply
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 494, 16, 494, 56);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 494, 16, 494, 56);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 496, 17, 496, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 497, 17, 497, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 495, 13, 498, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 499, 16, 499, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 499, 16, 499, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 501, 17, 501, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 502, 17, 502, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 500, 13, 503, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 504, 16, 504, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 506, 17, 506, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 505, 13, 507, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_boolean";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Return_boolean =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Return_boolean.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Return_boolean'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_boolean";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Return_boolean",
                new Type_39() {
                  public Type_40 apply() {
                    return
                      new Type_40() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Return_boolean
                                     .apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Return_boolean'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Return_boolean";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_41 body_of_Return_long =
  new Type_41() {
    public Type_42 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_42() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 513, 9, 513, 31); }
                if (DEBUG) { tracePoint(sourceFile, 515, 9, 515, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 516, 9, 516, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(2)));
                if (DEBUG) { tracePoint(sourceFile, 517, 9, 517, 55); }
                TTCN3_giop_port
                .send(TTCN3_request
                      .apply(TTCN3_req_id, new CharString("return_long")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 518, 9, 518, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 519, 9, 535, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_long_reply
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 521, 16, 521, 53);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 521, 16, 521, 53);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 523, 17, 523, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 524, 17, 524, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 522, 13, 525, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 526, 16, 526, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 526, 16, 526, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 528, 17, 528, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 529, 17, 529, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 527, 13, 530, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 531, 16, 531, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 533, 17, 533, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 532, 13, 534, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_long";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Return_long =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Return_long.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Return_long'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_long";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Return_long",
                new Type_43() {
                  public Type_44 apply() {
                    return
                      new Type_44() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Return_long.apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Return_long'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Return_long";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_45 body_of_Return_longlong =
  new Type_45() {
    public Type_46 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_46() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 540, 9, 540, 31); }
                if (DEBUG) { tracePoint(sourceFile, 542, 9, 542, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 543, 9, 543, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(2)));
                if (DEBUG) { tracePoint(sourceFile, 544, 9, 544, 59); }
                TTCN3_giop_port
                .send(TTCN3_request
                      .apply(TTCN3_req_id, new CharString("return_longlong")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 545, 9, 545, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 546, 9, 562, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_longlong_reply
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 548, 16, 548, 57);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 548, 16, 548, 57);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 550, 17, 550, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 551, 17, 551, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 549, 13, 552, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 553, 16, 553, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 553, 16, 553, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 555, 17, 555, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 556, 17, 556, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 554, 13, 557, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 558, 16, 558, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 560, 17, 560, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 559, 13, 561, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_longlong";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Return_longlong =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Return_longlong.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Return_longlong'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_longlong";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Return_longlong",
                new Type_47() {
                  public Type_48 apply() {
                    return
                      new Type_48() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Return_longlong
                                     .apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Return_longlong'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Return_longlong";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_49 body_of_Return_short =
  new Type_49() {
    public Type_50 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_50() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 566, 9, 566, 31); }
                if (DEBUG) { tracePoint(sourceFile, 568, 9, 568, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 569, 9, 569, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(2)));
                if (DEBUG) { tracePoint(sourceFile, 570, 9, 570, 56); }
                TTCN3_giop_port
                .send(TTCN3_request
                      .apply(TTCN3_req_id, new CharString("return_short")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 571, 9, 571, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 572, 9, 588, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_short_reply
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 574, 16, 574, 54);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 574, 16, 574, 54);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 576, 17, 576, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 577, 17, 577, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 575, 13, 578, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 579, 16, 579, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 579, 16, 579, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 581, 17, 581, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 582, 17, 582, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 580, 13, 583, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 584, 16, 584, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 586, 17, 586, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 585, 13, 587, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_short";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Return_short =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Return_short.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Return_short'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_short";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Return_short",
                new Type_51() {
                  public Type_52 apply() {
                    return
                      new Type_52() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Return_short.apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Return_short'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Return_short";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_53 body_of_Return_octet =
  new Type_53() {
    public Type_54 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_54() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 592, 9, 592, 31); }
                if (DEBUG) { tracePoint(sourceFile, 594, 9, 594, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 595, 9, 595, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(2)));
                if (DEBUG) { tracePoint(sourceFile, 596, 9, 596, 56); }
                TTCN3_giop_port
                .send(TTCN3_request
                      .apply(TTCN3_req_id, new CharString("return_octet")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 597, 9, 597, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 598, 9, 614, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_octet_reply
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 600, 16, 600, 54);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 600, 16, 600, 54);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 602, 17, 602, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 603, 17, 603, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 601, 13, 604, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 605, 16, 605, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 605, 16, 605, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 607, 17, 607, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 608, 17, 608, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 606, 13, 609, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 610, 16, 610, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 612, 17, 612, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 611, 13, 613, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_octet";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Return_octet =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Return_octet.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Return_octet'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_octet";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Return_octet",
                new Type_55() {
                  public Type_56 apply() {
                    return
                      new Type_56() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Return_octet.apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Return_octet'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Return_octet";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_57 body_of_Return_float =
  new Type_57() {
    public Type_58 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_58() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 618, 9, 618, 31); }
                if (DEBUG) { tracePoint(sourceFile, 620, 9, 620, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 621, 9, 621, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(2)));
                if (DEBUG) { tracePoint(sourceFile, 622, 9, 622, 56); }
                TTCN3_giop_port
                .send(TTCN3_request
                      .apply(TTCN3_req_id, new CharString("return_float")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 623, 9, 623, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 624, 9, 640, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_float_reply
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 626, 16, 626, 54);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 626, 16, 626, 54);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 628, 17, 628, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 629, 17, 629, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 627, 13, 630, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 631, 16, 631, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 631, 16, 631, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 633, 17, 633, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 634, 17, 634, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 632, 13, 635, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 636, 16, 636, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 638, 17, 638, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 637, 13, 639, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_float";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Return_float =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Return_float.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Return_float'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_float";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Return_float",
                new Type_59() {
                  public Type_60 apply() {
                    return
                      new Type_60() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Return_float.apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Return_float'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Return_float";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_61 body_of_Return_double =
  new Type_61() {
    public Type_62 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_62() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 644, 9, 644, 31); }
                if (DEBUG) { tracePoint(sourceFile, 646, 9, 646, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 647, 9, 647, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(2)));
                if (DEBUG) { tracePoint(sourceFile, 648, 9, 648, 57); }
                TTCN3_giop_port
                .send(TTCN3_request
                      .apply(TTCN3_req_id, new CharString("return_double")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 649, 9, 649, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 650, 9, 666, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_double_reply
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 652, 16, 652, 55);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 652, 16, 652, 55);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 654, 17, 654, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 655, 17, 655, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 653, 13, 656, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 657, 16, 657, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 657, 16, 657, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 659, 17, 659, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 660, 17, 660, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 658, 13, 661, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 662, 16, 662, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 664, 17, 664, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 663, 13, 665, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_double";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Return_double =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Return_double.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Return_double'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_double";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Return_double",
                new Type_63() {
                  public Type_64 apply() {
                    return
                      new Type_64() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Return_double.apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Return_double'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Return_double";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_65 body_of_Return_char =
  new Type_65() {
    public Type_66 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_66() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 670, 9, 670, 31); }
                if (DEBUG) { tracePoint(sourceFile, 672, 9, 672, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 673, 9, 673, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(2)));
                if (DEBUG) { tracePoint(sourceFile, 674, 9, 674, 55); }
                TTCN3_giop_port
                .send(TTCN3_request
                      .apply(TTCN3_req_id, new CharString("return_char")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 675, 9, 675, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 676, 9, 692, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_char_reply
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 678, 16, 678, 53);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 678, 16, 678, 53);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 680, 17, 680, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 681, 17, 681, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 679, 13, 682, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 683, 16, 683, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 683, 16, 683, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 685, 17, 685, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 686, 17, 686, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 684, 13, 687, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 688, 16, 688, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 690, 17, 690, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 689, 13, 691, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_char";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Return_char =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Return_char.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Return_char'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_char";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Return_char",
                new Type_67() {
                  public Type_68 apply() {
                    return
                      new Type_68() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Return_char.apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Return_char'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Return_char";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_69 body_of_Return_string =
  new Type_69() {
    public Type_70 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_70() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 696, 9, 696, 31); }
                if (DEBUG) { tracePoint(sourceFile, 698, 9, 698, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 699, 9, 699, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(2)));
                if (DEBUG) { tracePoint(sourceFile, 700, 9, 700, 57); }
                TTCN3_giop_port
                .send(TTCN3_request
                      .apply(TTCN3_req_id, new CharString("return_string")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 701, 9, 701, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 702, 9, 718, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_string_reply
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 704, 16, 704, 55);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 704, 16, 704, 55);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 706, 17, 706, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 707, 17, 707, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 705, 13, 708, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 709, 16, 709, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 709, 16, 709, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 711, 17, 711, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 712, 17, 712, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 710, 13, 713, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 714, 16, 714, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 716, 17, 716, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 715, 13, 717, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_string";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Return_string =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Return_string.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Return_string'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_string";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Return_string",
                new Type_71() {
                  public Type_72 apply() {
                    return
                      new Type_72() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Return_string.apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Return_string'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Return_string";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_73 body_of_Return_enum =
  new Type_73() {
    public Type_74 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_74() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 722, 9, 722, 31); }
                if (DEBUG) { tracePoint(sourceFile, 724, 9, 724, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 725, 9, 725, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(2)));
                if (DEBUG) { tracePoint(sourceFile, 726, 9, 726, 55); }
                TTCN3_giop_port
                .send(TTCN3_request
                      .apply(TTCN3_req_id, new CharString("return_enum")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 727, 9, 727, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 728, 9, 744, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_enum_reply
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 730, 16, 730, 53);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 730, 16, 730, 53);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 732, 17, 732, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 733, 17, 733, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 731, 13, 734, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 735, 16, 735, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 735, 16, 735, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 737, 17, 737, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 738, 17, 738, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 736, 13, 739, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 740, 16, 740, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 742, 17, 742, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 741, 13, 743, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_enum";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Return_enum =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Return_enum.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Return_enum'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Return_enum";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Return_enum",
                new Type_75() {
                  public Type_76 apply() {
                    return
                      new Type_76() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Return_enum.apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Return_enum'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Return_enum";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_77 body_of_Locate_object =
  new Type_77() {
    public Type_78 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_78() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 748, 9, 748, 31); }
                if (DEBUG) { tracePoint(sourceFile, 750, 9, 750, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 751, 9, 751, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(3)));
                if (DEBUG) { tracePoint(sourceFile, 752, 9, 752, 46); }
                TTCN3_giop_port
                .send(TTCN3_locateRequest.apply(TTCN3_req_id), null);
                if (DEBUG) { tracePoint(sourceFile, 753, 9, 753, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 754, 9, 770, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_locateReply
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 756, 16, 756, 54);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 756, 16, 756, 54);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 758, 17, 758, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 759, 17, 759, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 757, 13, 760, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 761, 16, 761, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 761, 16, 761, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 763, 17, 763, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 764, 17, 764, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 762, 13, 765, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 766, 16, 766, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 768, 17, 768, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 767, 13, 769, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Locate_object";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Locate_object =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Locate_object.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Locate_object'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Locate_object";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Locate_object",
                new Type_79() {
                  public Type_80 apply() {
                    return
                      new Type_80() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Locate_object.apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Locate_object'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Locate_object";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_81 body_of_Locate_unknown_object =
  new Type_81() {
    public Type_82 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_82() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 774, 7, 774, 29); }
                if (DEBUG) { tracePoint(sourceFile, 776, 9, 776, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 777, 9, 777, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(3)));
                if (DEBUG) { tracePoint(sourceFile, 778, 9, 778, 54); }
                TTCN3_giop_port
                .send(TTCN3_locateRequest_unknown.apply(TTCN3_req_id), null);
                if (DEBUG) { tracePoint(sourceFile, 779, 9, 779, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 780, 9, 796, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_locateReply_unknown
                                         .apply(TTCN3_req_id))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 782, 16, 782, 62);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 782, 16, 782, 62);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 784, 17, 784, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 785, 17, 785, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 783, 13, 786, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 787, 16, 787, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 787, 16, 787, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 789, 17, 789, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 790, 17, 790, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 788, 13, 791, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 792, 16, 792, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 794, 17, 794, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 793, 13, 795, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Locate_unknown_object";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Locate_unknown_object =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return
                    mtc.run(body_of_Locate_unknown_object.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Locate_unknown_object'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Locate_unknown_object";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Locate_unknown_object",
                new Type_83() {
                  public Type_84 apply() {
                    return
                      new Type_84() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Locate_unknown_object
                                     .apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Locate_unknown_object'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Locate_unknown_object";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_85 body_of_Wrong_magic =
  new Type_85() {
    public Type_86 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_86() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 801, 9, 801, 31); }
                if (DEBUG) { tracePoint(sourceFile, 803, 9, 803, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 804, 9, 804, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(4)));
                if (DEBUG) { tracePoint(sourceFile, 805, 9, 805, 62); }
                TTCN3_giop_port
                .send(TTCN3_wrong_magic
                      .apply(TTCN3_req_id, new CharString("return_boolean")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 806, 9, 806, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 807, 9, 833, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_message_error_1_0)) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 809, 14, 809, 50);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 809, 14, 809, 50);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 811, 13, 811, 29);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 812, 13, 812, 30);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 810, 11, 813, 12);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(TTCN3_message_error_1_1)) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 814, 14, 814, 50);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 814, 14, 814, 50);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 816, 13, 816, 29);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 817, 13, 817, 30);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 815, 11, 818, 12);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(TTCN3_message_error_1_2)) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 819, 14, 819, 50);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 819, 14, 819, 50);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 821, 13, 821, 29);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 822, 13, 822, 30);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 820, 11, 823, 12);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 824, 14, 824, 31);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 824, 14, 824, 31);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 826, 13, 826, 29);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 827, 13, 827, 30);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 825, 11, 828, 12);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 829, 14, 829, 33);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 831, 13, 831, 30);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 830, 11, 832, 12);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Wrong_magic";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Wrong_magic =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Wrong_magic.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Wrong_magic'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Wrong_magic";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Wrong_magic",
                new Type_87() {
                  public Type_88 apply() {
                    return
                      new Type_88() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Wrong_magic.apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Wrong_magic'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Wrong_magic";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_89 body_of_Wrong_version =
  new Type_89() {
    public Type_90 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_90() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 838, 9, 838, 31); }
                if (DEBUG) { tracePoint(sourceFile, 840, 9, 840, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 841, 9, 841, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(4)));
                if (DEBUG) { tracePoint(sourceFile, 842, 9, 842, 64); }
                TTCN3_giop_port
                .send(TTCN3_wrong_version
                      .apply(TTCN3_req_id, new CharString("return_boolean")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 843, 9, 843, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 844, 9, 860, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_message_error_1_2)) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 846, 16, 846, 52);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 846, 16, 846, 52);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 848, 17, 848, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 849, 17, 849, 34);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 847, 13, 850, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 851, 16, 851, 33);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 851, 16, 851, 33);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 853, 17, 853, 33);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 854, 17, 854, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 852, 13, 855, 14);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 856, 16, 856, 35);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 858, 17, 858, 34);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 857, 13, 859, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Wrong_version";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Wrong_version =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Wrong_version.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Wrong_version'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Wrong_version";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Wrong_version",
                new Type_91() {
                  public Type_92 apply() {
                    return
                      new Type_92() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Wrong_version.apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Wrong_version'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Wrong_version";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final Type_93 body_of_Wrong_messagetype =
  new Type_93() {
    public Type_94 apply(Data arg0, Data arg1) {
      final TTCN3_MTC_Type mtc =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg0);
      final TTCN3_MTC_Type system =
        (TTCN3_MTC_Type)new TTCN3_MTC_Type().init(arg1);
      return
        new Type_94() {
          public void on(final Interface_TTCN3_MTC_Type self) {
            try {
              final PortInterface TTCN3_giop_port = self.get_TTCN3_giop_port();
              final
                TimerReference
                  TTCN3_Timer_reply = self.get_TTCN3_Timer_reply();
              execute(self);
              {
                final TTCN3_CORBA_ULong TTCN3_req_id = new TTCN3_CORBA_ULong();
                if (DEBUG) { tracePoint(sourceFile, 865, 9, 865, 31); }
                if (DEBUG) { tracePoint(sourceFile, 867, 9, 867, 46); }
                ((TTCN3_MTC_Type)getSelf()).get_TTCN3_giop_port()
                .map(((TTCN3_MTC_Type)getSystem()).get_TTCN3_giop_port());
                if (DEBUG) { tracePoint(sourceFile, 868, 9, 868, 20); }
                TTCN3_req_id
                .set((TTCN3_CORBA_ULong)new TTCN3_CORBA_ULong()
                                        .init(new Int(4)));
                if (DEBUG) { tracePoint(sourceFile, 869, 9, 869, 68); }
                TTCN3_giop_port
                .send(TTCN3_wrong_messagetype
                      .apply(TTCN3_req_id, new CharString("return_boolean")),
                      null);
                if (DEBUG) { tracePoint(sourceFile, 870, 9, 870, 31); }
                TTCN3_Timer_reply.start(5.0F);
                if (DEBUG) { tracePoint(sourceFile, 871, 9, 897, 10); }
                self
                .alt(new
                       Runnable()
                       {
                         public void run() {
                           if (TTCN3_giop_port
                               .received(TTCN3_message_error_1_2)) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 873, 14, 873, 50);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 873, 14, 873, 50);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 875, 13, 875, 29);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 876, 13, 876, 30);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 874, 11, 877, 12);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(TTCN3_message_error_1_1)) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 878, 14, 878, 50);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 878, 14, 878, 50);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 880, 13, 880, 29);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 881, 13, 881, 30);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 879, 11, 882, 12);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(TTCN3_message_error_1_0)) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 883, 14, 883, 50);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 883, 14, 883, 50);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 885, 13, 885, 29);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 886, 13, 886, 30);
                               }
                               self.setVerdict(Verdict.pass);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 884, 11, 887, 12);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_giop_port
                               .received(((Present)
                                          new Present().toPresent()))) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 888, 14, 888, 31);
                             }
                             self.choose(true);
                             if (DEBUG) {
                               tracePoint(sourceFile, 888, 14, 888, 31);
                             }
                             TTCN3_giop_port.consume();
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 890, 13, 890, 29);
                               }
                               TTCN3_Timer_reply.stop();
                               if (DEBUG) {
                                 tracePoint(sourceFile, 891, 13, 891, 30);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 889, 11, 892, 12);
                             }
                             throw Module.Break;
                           }
                           if (TTCN3_Timer_reply
                               .timeout(self.currentSnapshot())) {
                             if (DEBUG) {
                               tracePoint(sourceFile, 893, 14, 893, 33);
                             }
                             self.choose(true);
                             {
                               if (DEBUG) {
                                 tracePoint(sourceFile, 895, 13, 895, 30);
                               }
                               self.setVerdict(Verdict.fail);
                             }
                             if (DEBUG) {
                               tracePoint(sourceFile, 894, 11, 896, 14);
                             }
                             throw Module.Break;
                           }
                         }
                       });
              }
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Wrong_messagetype";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
        };
    }
  };
  public final Type_38 TTCN3_Wrong_messagetype =
  new Type_38() {
    public TestCaseBehavior apply() {
      return
        new TestCaseBehavior() {
          public Verdict on(final ComponentInterface self) {
            try {
              {
                final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                final TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                mtc.set((TTCN3_MTC_Type)new TTCN3_MTC_Type().create().toMTC());
                system.set(((TTCN3_MTC_Type)getSystem()));
                if (true) {
                  return mtc.run(body_of_Wrong_messagetype.apply(mtc, system));
                }
              }
              throw
                new
                  TestCaseError("reached end of non-void function 'Wrong_messagetype'");
            } catch(TestCaseError e) {
              traceHandle(e);
              throw e;
            } catch(RuntimeException e) {
              traceHandle(e);
              throw e;
            }
          }
          public String getName() {
            return "Wrong_messagetype";
          }
          public Collection getTimerParameters() {
            return Arrays.asList(new TimerReference[] {});
          }
          public Data execute(ComponentInterface self, float time) {
            final Thread killer = killMeAfter(time);
            if (killer != null) { killer.start(); }
            try {
              return this.on((ComponentInterface)self);
            } finally {
              if (killer != null) { killer.interrupt(); }
            }
          }
        };
    }
  };
  {
    addTestCase("Wrong_messagetype",
                new Type_95() {
                  public Type_96 apply() {
                    return
                      new Type_96() {
                        public
                          TTCN3_MTC_Type on(final ComponentInterface self) {
                          try {
                            {
                              final TTCN3_MTC_Type mtc = new TTCN3_MTC_Type();
                              final
                                TTCN3_MTC_Type system = new TTCN3_MTC_Type();
                              mtc
                              .set((TTCN3_MTC_Type)new TTCN3_MTC_Type()
                                                   .create()
                                                   .toMTC());
                              system.set(((TTCN3_MTC_Type)getSystem()));
                              mtc
                              .start(body_of_Wrong_messagetype
                                     .apply(mtc, system));
                              if (true) { return mtc; }
                            }
                            throw
                              new
                                TestCaseError("reached end of non-void function 'asynchronous_Wrong_messagetype'");
                          } catch(TestCaseError e) {
                            traceHandle(e);
                            throw e;
                          } catch(RuntimeException e) {
                            traceHandle(e);
                            throw e;
                          }
                        }
                        public String getName() {
                          return "Wrong_messagetype";
                        }
                        public Collection getTimerParameters() {
                          return Arrays.asList(new TimerReference[] {});
                        }
                      };
                  }
                });
  }
  public final TestCaseBehavior control =
  new TestCaseBehavior() {
    public Verdict on(final ComponentInterface self) {
      try {
        {
          {
            if (DEBUG) { tracePoint(sourceFile, 903, 16, 903, 41); }
            TTCN3_Return_boolean.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 904, 16, 904, 38); }
            TTCN3_Return_long.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 905, 16, 905, 42); }
            TTCN3_Return_longlong.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 906, 16, 906, 39); }
            TTCN3_Return_short.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 907, 16, 907, 39); }
            TTCN3_Return_octet.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 908, 16, 908, 39); }
            TTCN3_Return_float.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 909, 16, 909, 40); }
            TTCN3_Return_double.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 910, 16, 910, 38); }
            TTCN3_Return_char.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 911, 16, 911, 40); }
            TTCN3_Return_string.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 912, 16, 912, 38); }
            TTCN3_Return_enum.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 913, 16, 913, 40); }
            TTCN3_Locate_object.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 914, 16, 914, 48); }
            TTCN3_Locate_unknown_object.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 915, 16, 915, 38); }
            TTCN3_Wrong_magic.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 916, 16, 916, 40); }
            TTCN3_Wrong_version.apply().on(self);
            if (DEBUG) { tracePoint(sourceFile, 917, 16, 917, 44); }
            TTCN3_Wrong_messagetype.apply().on(self);
          }
          if (true) { return self.getVerdict(); }
        }
        throw new TestCaseError("reached end of non-void function 'control'");
      } catch(TestCaseError e) {
        traceHandle(e);
        throw e;
      } catch(RuntimeException e) {
        traceHandle(e);
        throw e;
      }
    }
    public String getName() {
      return "control";
    }
    public Collection getTimerParameters() {
      return Arrays.asList(new TimerReference[] {});
    }
    public Data execute(ComponentInterface self, float time) {
      final Thread killer = killMeAfter(time);
      if (killer != null) { killer.start(); }
      try {
        return this.on((ComponentInterface)self);
      } finally {
        if (killer != null) { killer.interrupt(); }
      }
    }
  };
  public void control() {
    final ComponentReference tester = new ComponentReference();
    tester.createEntity();
    tester.toControl().start(control);
    tester.join();
  }
}
