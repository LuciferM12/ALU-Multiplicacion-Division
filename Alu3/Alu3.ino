const int S0 = 5, S1 = 4, S2 = 3, S3 = 2; //Para configurar las combinaciones
const int a[4] = {10,11,12,13}, b[4] = {6,7,8,9}; //Los operandos
const int f[5] = {A0, A1, A2, A3, A4}; //El resultado de la ALU
const int TX = 1, RX = 0;  //Salidas a terminal
int aux = 0, op = 0, aux2 = 0, aux3 = 0, aux4 = 0; //Para realizar operaciones




void setup() 
{
  //Modo de pines
  for(int i = 0; i < 4; i++)
  {
    pinMode(a[i], OUTPUT);
    pinMode(b[i], OUTPUT);
    pinMode(f[i], INPUT);
  }
  pinMode(f[4],INPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(TX, INPUT);
  pinMode(RX, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  //Arreglo para manipular los bits
  int arreglo[4] = {0,0,0,0};
  int arreglo2[4] = {0,0,0,0};
  int result[5] = {0,0,0,0,0}; 
  //Para conversion de int a bit
  int cont = 0;
  int tot = 0;
  int exp = 16;
  //Para ver si es la primera vez
  bool bandera = true;
  int mult = 0;
  //Inicia 
  if(Serial.available() > 0)
  {
    Serial.println("Introduce un valor para A: ");
    aux = Serial.parseInt();
    Serial.println((String) "El numero ingresado es: " + aux);
    while (Serial.available() > 0) {
      Serial.read(); //Limpia el buffer
    }
    Serial.println("Introduce un valor para B: ");
    while (Serial.available() == 0) {
      // Espera a que haya datos disponibles
    }
    aux2 = Serial.parseInt();
    aux3 = aux2;
    Serial.println((String) "El numero ingresado es: " + aux2);
    while (Serial.available() > 0) {
      Serial.read(); //Limpia el buffer
    }

    Serial.println("Que desea realizar: 1.Multiplicacion, 2.Division");
    while (Serial.available() == 0) {
      // Espera a que haya datos disponibles
    }
    op = Serial.parseInt();
    while (Serial.available() > 0) {
      Serial.read(); //Limpia el buffer
    }
    //Coloca las combinaciones de acuerdo a la operacion a realizar
    Serial.println((String) "El numero ingresado es: " + op);
    if(op == 1){
      digitalWrite(S0,1);
      digitalWrite(S1,0);
      digitalWrite(S2,0);
      digitalWrite(S3,1);
    }else if(op == 2){
      digitalWrite(S0,0);
      digitalWrite(S1,1);
      digitalWrite(S2,1);
      digitalWrite(S3,0);
    }
    aux3 = aux; //Variables secundarias
    aux4 = aux2;
    if(aux2 == 1) //Conversiones a bits
    {
      arreglo2[0] = 1;
    }
    else
    {
      if(aux2 == 2)
      {
        arreglo2[1] = 1;
      }
      else
      {
        if(aux2 == 3)
        {
          arreglo2[0] = 1;
          arreglo2[1] = 1;
        }
        else
        {
          while(aux2 > 0) //Convierte decimal a binario
          {
            arreglo2[cont] = aux2 % 2;
            cont++;
            aux2 /= 2;
          }
        }
      }
    }
    
    cont = 0;
    if(aux == 1)
    {
      arreglo[0] = 1;
    }
    else
    {
      if(aux == 2)
      {
        arreglo[1] = 1;
      }
      else
      {
        if(aux == 3)
        {
          arreglo[0] = 1;
          arreglo[1] = 1;
        }
        else
        {
          while(aux > 0)
          {
            arreglo[cont] = aux % 2;
            cont++;
            aux /= 2;
          }
        }
      }
    }
    if(op == 1)
    {
      if(aux4 == 1) //Multiplicacion * 1
      {
        Serial.println((String)"El resultado es " + aux3);
        Serial.println("---------------------------------");
      }
      else
      {
        if(aux4 == 0) //Multiplicacion * 0
        {
          Serial.println((String)"El resultado es " + 0);
          Serial.println("---------------------------------");
        }
        else
        {
          for(int i = 0; i < 4; i++) //Envia operandos a ALU
          {
            digitalWrite(a[i],arreglo[3-i]);
            digitalWrite(b[i],arreglo[3-i]);
          }
          aux4 -= 2;
          while(aux4 > 0)
          {
            for(int i = 0; i < 4; i++)
            {
              digitalWrite(b[i],digitalRead(f[3-i])); //Envia el resultado de la op anterior a la ALU
            }
            aux4-=1; 
          }
          for(int i = 0; i < 5; i++)
          {
            if(i == 0)
            {
              if(!(digitalRead(f[4]))) //Para arreglar el acarreo :(
              {
                tot += 1*exp;
              }
            }
            else
            {
              tot += digitalRead(f[4-i])*exp;
            }
            exp /=2;
          }
          Serial.println((String) "El resultado es: " + tot );
          Serial.println("---------------------------------");
        }
      }
    }
    else
    {
      if(aux4 == 0 || aux3 == 0) //Divisiones entre 0
      {
        Serial.println((String)"El resultado es " + 0);
        Serial.println("---------------------------------");
      }
      else
      {
        if(aux4 > aux3)
        {
          Serial.println((String) "El resultado es " + 0); //Division entre numero mayor
          Serial.println("---------------------------------");
        }
        else
        {
          for(int i = 0; i < 4; i++)
          {
            digitalWrite(a[i],arreglo[3-i]); //Envia a ALU
            digitalWrite(b[i],arreglo2[3-i]);
          }


          aux3 -= aux4;
          mult += 1;
          while(aux3>=aux4)
          {
            /*exp = 8;
            tot = 0;
            for(int i = 0; i < 4; i++)
            {
              tot += digitalRead(f[3-i])*exp; //Lee el resultado y lo vuelve decimal
              exp /=2;
              
            }*/
            tot = binDecimal(4, 8, 1, arreglo);
            tot += 1; //Arregla el -1 de la ALU
            cont = 0;
            int arreglo3[] ={0,0,0,0};
            while(tot > 0) //Vuelve a binario
            {
              arreglo3[cont] = tot % 2;
              cont++;
              tot /= 2;
            }
            for(int i = 0; i < 4; i++)
            {
              digitalWrite(a[i],arreglo3[3-i]); //Lo regresa a la ALU

            }
            if(bandera) //Primera vuelta
            {
              bandera = false;
            }
            else
            {
              /*tot = 0;
              exp = 8;
              for(int i = 0; i < 4; i++)
              {
                  tot += arreglo3[3-i]*exp;
                  exp /=2;
              }
              */
              tot = binDecimal(4, 8, 2, arreglo3);
              aux3 =tot; //Actualiza aux3 para ver si debe repetir el proceso
              mult += 1;
              
            }
            
          }
          Serial.println((String) "El resultado es: " + mult );
          Serial.println("---------------------------------");
          mult = 0;
          tot = 0;
        }
      }
    }
  }  
  


   
}


int binDecimal(int bits, int exp, int opc, int arreglo[])
{
  int tot = 0;
  for(int i = 0; i < bits; i++)
  {
    if(opc == 1) 
    {
      tot += digitalRead(f[(bits-1)-i])*exp;
    }
    else
    {
      tot+= arreglo[(bits-1) -i]*exp; //Lee el resultado y lo vuelve decimal
    }
    exp /=2;
    
  }
  return tot;
}
