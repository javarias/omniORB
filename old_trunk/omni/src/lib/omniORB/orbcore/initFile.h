// -*- Mode: C++; -*-
//                            Package   : omniORB2
// initFile.h                 Created on: 10/01/97
//                            Author    : Eoin Carroll  (ewc)
//
// Copyright (C) Olivetti Research Limited, 1996
//
// Description:
//	*** PROPRIETORY INTERFACE ***
//	

/*
  $Log$
  */


// Class to read and store constants from initialization file.
// Used for ATMos and UNIX.

#ifndef __INITFILE_H__
#define __INITFILE_H__


#define MAX_CONFIG 10   


class initFile
{
public:
  
  initFile();
  virtual ~initFile();

  void initialize();
  

private:
  char* fData;
  long fsize;

  inline void invref(char* entryname);
  inline void multerr(char* entryname);      
  inline void dataerr(char* entryname);
  inline void parseerr();
  

  int getnextentry(char*& entryname, char*& data);
 

public:
  
  omniObject* NameService;
};


#endif  
        // __INITFILE_H__
