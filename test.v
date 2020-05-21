module top(\I[3] ,\I[2] ,\I[1] ,\I[0] ,\O[2] ,\O[1] ,\O[0] );
input \I[3] , \I[2] , \I[1] , \I[0] ;
output \O[2] , \O[1] , \O[0] ;

wire \40 , \41 ;

and \andno1 ( \40 , \I[1] , \I[0] );
and \andno2 ( \41 , \I[2] , \40 );
and \andno3 ( \O[1] , \I[3] , \41 );
or \orno1 ( \O[0] , \41, \I[3] );
or \orno2 ( \O[2] , \41, \40 );

endmodule

