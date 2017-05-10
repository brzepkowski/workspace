#!/bin/bash

java -cp .:../../../lib/org.apache.commons.codec-1.3.0.jar Main challenge aes-keystore.jck jceksaes message_1 message_2

java Zad_2

cryptogram=`cat messageB_enc`

java -cp .:../../../lib/org.apache.commons.codec-1.3.0.jar Main oracle encrypt aes-keystore.jck jceksaes message_2_modified

cryptogram2=`cat message_2_modified_enc`

if [ "$cryptogram" == "$cryptogram2" ]; then
	echo "1"
else
	echo "0"
fi
