module ul4(output wire[3:0] Out, input wire[3:0] A, input wire[3:0] B, input wire [1:0] S);

  /** 
    Instanciamos cuatro celdas lógicas para cada bit
    cl cl0 (Out[0], A[0], B[0], S); // cl_out[0] es la salida de la celda lógica 0
    cl cl1 (Out[1], A[1], B[1], S);
    cl cl2 (Out[2], A[2], B[2], S);
    cl cl3 (Out[3], A[3], B[3], S);
  */

  genvar i;
  generate
    for (i = 0; i < 4; i = i + 1) begin
      cl cl_inst (Out[i], A[i], B[i], S); // Instancia de la celda lógica
    end
  endgenerate
  
endmodule