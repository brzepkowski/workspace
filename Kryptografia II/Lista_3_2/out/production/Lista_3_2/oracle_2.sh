#!/bin/bash

java -cp .:../../../lib/org.apache.commons.codec-1.3.0.jar Main oracle decrypt aes-keystore.jck jceksaes input_enc
