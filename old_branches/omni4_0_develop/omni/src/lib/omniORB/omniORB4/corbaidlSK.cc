// This file is generated by omniidl (C++ backend)- omniORB_3_0. Do not edit.

size_t
CORBA::StructMember::_NP_alignedSize(size_t _initialoffset) const
{
  CORBA::ULong _msgsize = _initialoffset;
  _msgsize = name._NP_alignedSize(_msgsize);

  _msgsize = type._NP_alignedSize(_msgsize);

  _msgsize = type_def._NP_alignedSize(_msgsize);

  return _msgsize;
}

void
CORBA::StructMember::operator>>= (NetBufferedStream &_n) const
{
  name >>= _n;
  type >>= _n;
  type_def >>= _n;
  
}

void
CORBA::StructMember::operator<<= (NetBufferedStream &_n)
{
  name <<= _n;
  type <<= _n;
  type_def <<= _n;
  
}

void
CORBA::StructMember::operator>>= (MemBufferedStream &_n) const
{
  name >>= _n;
  type >>= _n;
  type_def >>= _n;
  
}

void
CORBA::StructMember::operator<<= (MemBufferedStream &_n)
{
  name <<= _n;
  type <<= _n;
  type_def <<= _n;
  
}

size_t
CORBA::UnionMember::_NP_alignedSize(size_t _initialoffset) const
{
  CORBA::ULong _msgsize = _initialoffset;
  _msgsize = name._NP_alignedSize(_msgsize);

  _msgsize = label._NP_alignedSize(_msgsize);

  _msgsize = type._NP_alignedSize(_msgsize);

  _msgsize = type_def._NP_alignedSize(_msgsize);

  return _msgsize;
}

void
CORBA::UnionMember::operator>>= (NetBufferedStream &_n) const
{
  name >>= _n;
  label >>= _n;
  type >>= _n;
  type_def >>= _n;
  
}

void
CORBA::UnionMember::operator<<= (NetBufferedStream &_n)
{
  name <<= _n;
  label <<= _n;
  type <<= _n;
  type_def <<= _n;
  
}

void
CORBA::UnionMember::operator>>= (MemBufferedStream &_n) const
{
  name >>= _n;
  label >>= _n;
  type >>= _n;
  type_def >>= _n;
  
}

void
CORBA::UnionMember::operator<<= (MemBufferedStream &_n)
{
  name <<= _n;
  label <<= _n;
  type <<= _n;
  type_def <<= _n;
  
}

