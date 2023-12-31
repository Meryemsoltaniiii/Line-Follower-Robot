#define MLav 2
#define MLar 4
#define MRav 5
#define MRar 7
#define enableR 3
#define enableL 11

static uint8_t timeout = 0;

static uint8_t c = 0, c1 = 0;
static uint8_t R_Fsensor_state_u8 = 0                                                                                                                                          ;
static uint8_t R_Rsensor_state_u8 = 0                                                                                                                                          ;
static uint8_t R_Lsensor_state_u8 = 0                                                                                                                                          ;
static uint8_t R_MRsensor_state_u8 = 0                                                                                                                                          ;
static uint8_t R_MLsensor_state_u8 = 0                                                                                                                                          ;
static uint32_t R_starttime_u32 = 0;

typedef enum
{
  R_start_en,
  R_triang_en,
  R_demi_cercle_en,
  R_disr_white_en,
  R_disr_white_en_part2,
  R_transition_en,
  R_square_en,
  R_square_part2_en,
  R_square_part3_en,
  R_square_part4_en,
  R_disr_black_en
} R_STATE;
R_STATE R_CURRENT_STATE =   R_start_en;

void R_vreadsensors();
void R_vforward();
void R_vbackward();
void R_vright();
void R_vleft();

void R_vreadsensors() {
  R_MLsensor_state_u8 = digitalRead(A1);
  R_MRsensor_state_u8 = digitalRead(A5);
  R_Lsensor_state_u8 = digitalRead(A2);
  R_Rsensor_state_u8 = digitalRead(A3);
  R_Fsensor_state_u8 = digitalRead(A4);
}

void R_vforward() {
  digitalWrite (MRav, HIGH);
  digitalWrite (MRar, LOW);
  analogWrite (enableR, 125);
  digitalWrite (MLav, HIGH);
  digitalWrite (MLar, LOW);
  analogWrite (enableL, 110);
}
void R_vbackward() {
  digitalWrite (MRav, LOW);
  digitalWrite (MRar, HIGH);
  analogWrite (enableR, 125);
  digitalWrite (MLav, LOW);
  digitalWrite (MLar, HIGH);
  analogWrite (enableL, 110);
}


void R_vstop() {
  digitalWrite (MRav, LOW);
  digitalWrite (MRar, LOW);
  analogWrite (enableR, 0);
  digitalWrite (MLav, LOW);
  digitalWrite (MLar, LOW);
  analogWrite (enableL, 0);
}


void R_vright2() {
  digitalWrite (MRav, LOW);
  digitalWrite (MRar, HIGH);
  analogWrite (enableR, 115 );
  digitalWrite (MLav, HIGH);
  digitalWrite (MLar, LOW);
  analogWrite (enableL, 90);
}
void R_vright() {
  digitalWrite (MRav, LOW);
  digitalWrite (MRar, LOW);
  analogWrite (enableR, 110 );
  digitalWrite (MLav, HIGH);
  digitalWrite (MLar, LOW);
  analogWrite (enableL, 85);
}
void R_vleft() {
  digitalWrite (MRav, HIGH);
  digitalWrite (MRar, LOW);
  analogWrite (enableR, 110 );
  digitalWrite (MLav, LOW);
  digitalWrite (MLar, HIGH);
  analogWrite (enableL, 85);
}

void R_vleft2() {
  digitalWrite (MRav, HIGH);
  digitalWrite (MRar, LOW);
  analogWrite (enableR, 115 );
  digitalWrite (MLav, LOW);
  digitalWrite (MLar, HIGH);
  analogWrite (enableL, 90);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  pinMode(MRav, OUTPUT);
  pinMode(MRar, OUTPUT);
  pinMode(MLav, OUTPUT);
  pinMode(MLar, OUTPUT);
  Serial.begin(9600);
}

void Black_Line_Follower()
{
  R_vreadsensors();
  if ((!R_Lsensor_state_u8) && R_MLsensor_state_u8 && R_MRsensor_state_u8 && (!R_Rsensor_state_u8)) {
    R_vforward();
  }
  else if (!R_Lsensor_state_u8 && !R_MLsensor_state_u8 && (R_MRsensor_state_u8) && (!R_Rsensor_state_u8)) {
    R_vright2();
  }
  else if (!R_Lsensor_state_u8 && R_MLsensor_state_u8 && (!R_MRsensor_state_u8) && (!R_Rsensor_state_u8)) {
    R_vleft2();
  }
  else if (R_Lsensor_state_u8 && R_MLsensor_state_u8 && (R_MRsensor_state_u8) && (!R_Rsensor_state_u8)) {
    while (!R_Fsensor_state_u8) {
      R_vreadsensors();
      R_vleft2();
    }
  }
  else if (!R_Lsensor_state_u8 && R_MLsensor_state_u8 && R_MRsensor_state_u8 && R_Rsensor_state_u8) {
    while (!R_Fsensor_state_u8) {
      R_vreadsensors();
      R_vright2();
    }
  }

}
void White_Line_Follower()
{
  R_vreadsensors();
  if ((R_Lsensor_state_u8) && !R_MLsensor_state_u8 && !R_MRsensor_state_u8 && (R_Rsensor_state_u8)) {
    R_vforward();
  }

  else if ( (!R_Rsensor_state_u8)) {
    while (R_Fsensor_state_u8) {
      R_vreadsensors();
      R_vright2();
    }
  }
  else if (!R_Lsensor_state_u8 /*&& (!R_MLsensor_state_u8 || !R_MRsensor_state_u8) && R_Rsensor_state_u8*/) {
    while (R_Fsensor_state_u8) {
      R_vreadsensors();
      R_vleft2();
    }
  }
  else if (!R_MLsensor_state_u8 && (R_MRsensor_state_u8)) {
    R_vleft2();
  }
  else if (R_MLsensor_state_u8 && (!R_MRsensor_state_u8)) {
    R_vright2();
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  switch (R_CURRENT_STATE)
  {

    case R_start_en:
      {
        if (millis() > 1000) {
          timeout = 1;
        }
        R_vreadsensors();
        //     Black_Line_Follower1();
        while ((!R_Rsensor_state_u8)) {
          R_vreadsensors();
          R_vforward();
        }

        while ((!R_Fsensor_state_u8)) {

          R_vreadsensors();
          R_vleft2();
        }

        R_vreadsensors();
        if ((R_Rsensor_state_u8) && (!R_Lsensor_state_u8))
        {
          R_CURRENT_STATE = R_triang_en;

          if (R_Fsensor_state_u8) {
            while (R_Fsensor_state_u8) {
              R_vreadsensors();
              R_vright2();
            }
          }

          while (!R_Fsensor_state_u8) {
            R_vreadsensors();
            R_vright2();
          }
          while (R_Lsensor_state_u8) {
            R_vreadsensors();
            R_vright2();
          }

        }
      } break;


    case R_triang_en:
      {

        Black_Line_Follower();

        R_vreadsensors();
        if ((R_Lsensor_state_u8) && (R_Rsensor_state_u8) && (R_Fsensor_state_u8)) {
          R_CURRENT_STATE =  R_demi_cercle_en;
        }

      } break;

    case R_demi_cercle_en:
      {
        R_vreadsensors();

        if ( R_MLsensor_state_u8 && R_MRsensor_state_u8 ) {
          R_vforward();
        }
        else if ( !R_MLsensor_state_u8 && (R_MRsensor_state_u8) ) {
          R_vright2();
        }
        else if ( R_MLsensor_state_u8 && (!R_MRsensor_state_u8) ) {
          R_vleft2();
        }
        else if (R_Rsensor_state_u8) {

          while (!R_Fsensor_state_u8) {
            R_vreadsensors();
            R_vleft2();
          }

          while ((!R_MLsensor_state_u8) && (!R_MRsensor_state_u8)) {
            R_vreadsensors();
            R_vforward();
          }

          R_CURRENT_STATE =   R_disr_white_en;

        }

      } break;

    case   R_disr_white_en:
      {

        R_vreadsensors();
        if (R_MLsensor_state_u8 && R_MRsensor_state_u8) {
          R_vforward();
        }
        else if ( !(R_MLsensor_state_u8) && (R_MRsensor_state_u8) && !(R_Lsensor_state_u8 )) {

          R_vright2();
          //  R_vright2();
        }
        else if (( R_MLsensor_state_u8 && (!R_MRsensor_state_u8))) {
          R_vleft2();

        }

        else if (R_Lsensor_state_u8 )
        {
          /*         while (R_Lsensor_state_u8) {
                     R_vreadsensors();
                     R_vleft2();
                   }

                   while (!R_Lsensor_state_u8) {
                     R_vreadsensors();
                     R_vleft2();
                   }
          */
          R_CURRENT_STATE =   R_disr_white_en_part2;
          while (R_Lsensor_state_u8) {
            R_vreadsensors();
            R_vforward();

          }
        }


      } break;

    case R_disr_white_en_part2: {
        R_vreadsensors();

        if (!R_Lsensor_state_u8) {
          R_vreadsensors();
          if ( R_MLsensor_state_u8 && R_MRsensor_state_u8 ) {
            R_vforward();
          }
          else if ( !R_MLsensor_state_u8 && (R_MRsensor_state_u8) ) {
            R_vright2();
          }
          else if ( R_MLsensor_state_u8 && (!R_MRsensor_state_u8) ) {
            R_vleft2();
          }
        }
        if (R_Lsensor_state_u8)
        {
          R_CURRENT_STATE =   R_disr_black_en;
          while (R_Lsensor_state_u8) {
            R_vreadsensors();
            R_vleft2();

          }
          while (!R_Fsensor_state_u8) {
            R_vreadsensors();
            R_vleft2();

          }
          //    while (1) R_vstop();
          R_vstop(); delay(100);
          /* while (!R_Fsensor_state_u8) {
                  R_vreadsensors();
                  R_vleft2();*/


          //  R_vstop();delay(500);
        }


      } break;


    case  R_disr_black_en:
      {
        R_vreadsensors();
        if ( R_MLsensor_state_u8 && R_MRsensor_state_u8 ) {
          R_vforward();
        }
        else if ( !R_MLsensor_state_u8 && (R_MRsensor_state_u8) ) {
          R_vright2();
        }
        else if ( R_MLsensor_state_u8 && (!R_MRsensor_state_u8) ) {
          R_vleft2();
        }
        else if ( !R_MLsensor_state_u8 && !R_MRsensor_state_u8 ) {

          R_CURRENT_STATE = R_square_en;
          //    while (1) R_vstop();
          R_vstop(); delay(100);
        }
      } break;
    case  R_transition_en:
      {
        R_vreadsensors();
        if ( !R_MLsensor_state_u8 && !R_MRsensor_state_u8 ) {
          R_vforward();
        }
        else if ( !R_MLsensor_state_u8 && (R_MRsensor_state_u8) ) {
          //   R_vleft2();
          R_vright2();
        }
        else if ( R_MLsensor_state_u8 && (!R_MRsensor_state_u8) ) {
          //   R_vright2();
          R_vleft2();
        }
        else if ( R_MLsensor_state_u8 && R_MRsensor_state_u8 && R_Rsensor_state_u8 ) {
          //      while (1) R_vstop();
          R_vstop(); delay(1000);
          R_CURRENT_STATE = R_square_en;
        }

      } break;
    case  R_square_en:
      {

        R_vforward(); delay(650);
        //                R_vreadsensors();
        //             if(R_Lsensor_state_u8 && !R_Rsensor_state_u8) {
        //                        R_vright2();
        //             }
        //           if( R_Fsensor_state_u8  ) {
        //            R_vforward();
        //          }
        while ( R_Fsensor_state_u8  ) {
          R_vreadsensors();
          R_vright2();
        }
        R_vstop(); delay(300);
        while ( R_Lsensor_state_u8  ||   R_Rsensor_state_u8 ) {
          R_vreadsensors();
          R_vforward();
        }
        R_vbackward(); delay(100);
        R_vstop(); delay(300);
        while ( R_Fsensor_state_u8  ) {
          R_vreadsensors();
          R_vleft2();
        }
        R_vstop(); delay(300);
        R_CURRENT_STATE = R_square_part2_en;
        //  while (1) R_vstop();
      } break;
    case  R_square_part2_en:
      {
        R_vreadsensors();
        White_Line_Follower();
        R_vreadsensors();
        if (R_MLsensor_state_u8 && R_MLsensor_state_u8 && R_MRsensor_state_u8 && R_Rsensor_state_u8) {
          R_vstop(); delay(200);
                    while (1) R_vstop();
          R_CURRENT_STATE = R_square_part3_en;
        }
      } break;
    case  R_square_part3_en:
      {

        R_vreadsensors();
        R_vforward();
        if (!R_Lsensor_state_u8) {
          while (!R_Lsensor_state_u8) {
            R_vforward();
          } 
        
        //  R_CURRENT_STATE = R_square_part4_en;
                   while (1) R_vstop();
        }
          else if (!R_MLsensor_state_u8 && (R_MRsensor_state_u8)) {
    R_vleft2();
  }
  else if (R_MLsensor_state_u8 && (!R_MRsensor_state_u8)) {
    R_vright2();
  }

      } break;
    case  R_square_part4_en:
      {
                R_vreadsensors();
        White_Line_Follower();
      } break;

      default: {
          //while (1) R_vstop();
        }
      }
  }
