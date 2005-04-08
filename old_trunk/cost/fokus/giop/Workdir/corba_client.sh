#!/bin/sh

CP=../lib/classes:/usr/local/orbs/orbacus/JOB-4.1/lib/OB.jar

echo "Starting the java client emulation..."

/usr/local/java/jdk1.3.1_01/bin/java -classpath $CP transferdata.Client 
