#!/usr/bin/bash
./chall
int () {                                                                                                                                                                                                
./chall $LINENO 999999                                                                                                                                                                                  
exit                                                                                                                                                                                                    
}                                                                                                                                                                                                       
trap "int" INT                                                                                                                                                                                          
./chall $LINENO 9823                                                                                                                                                                                    