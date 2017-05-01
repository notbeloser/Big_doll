#include "big_doll.h"

doll doll_default_setting(){
	doll d;
	d.l_eye.channel_x=0;
	d.l_eye.channel_x_gpio=13;
	d.l_eye.channel_y=1;
	d.l_eye.channel_y_gpio=12;
	d.l_eye.angle=0;
	d.l_eye.r=0;
	d.l_eye.change_time_ms=300;
	d.l_eye.rev=0;

	d.r_eye.channel_x=2;
	d.r_eye.channel_x_gpio=17;
	d.r_eye.channel_y=3;
	d.r_eye.channel_y_gpio=27;
	d.r_eye.angle=20;
	d.r_eye.r=0;
	d.r_eye.change_time_ms=300;
	d.r_eye.rev=1;

	d.l_ear.channel=4;
	d.l_ear.channel_gpio=26;
	d.l_ear.angle=20;
	d.l_ear.change_time_ms=300;
	d.l_ear.rev=1;

	d.r_ear.channel=5;
	d.r_ear.channel_gpio=25;
	d.r_ear.angle=0;
	d.r_ear.change_time_ms=300;
	d.r_ear.rev=0;

	d.l_bow.channel_angle=6;
	d.l_bow.channel_angle_gpio=33;
	d.l_bow.channel_y=7;
	d.l_bow.channel_y_gpio=32;
	d.l_bow.angle=0;
	d.l_bow.y=0;
	d.l_bow.change_time_ms=300;
	d.l_bow.rev=1;

	d.r_bow.channel_angle=8;
	d.r_bow.channel_angle_gpio=15;
	d.r_bow.channel_y=9;
	d.r_bow.channel_y_gpio=2;
	d.r_bow.angle=0;
	d.r_bow.y=0;
	d.r_bow.change_time_ms=300;
	d.r_bow.rev=0;

	d.c_mouth.angle=0;
	d.c_mouth.change_time_ms=300;
	d.c_mouth.channel=10;
	d.c_mouth.channel_gpio=4;
	d.c_mouth.side=0;
	d.c_mouth.rev=0;

	d.l_mouth.angle=0;
	d.l_mouth.change_time_ms=300;
	d.l_mouth.channel=11;
	d.l_mouth.channel_gpio=16;
	d.l_mouth.side=1;
	d.l_mouth.rev=0;

	d.r_mouth.angle=0;
	d.r_mouth.change_time_ms=300;
	d.r_mouth.channel=12;
	d.r_mouth.channel_gpio=5;
	d.r_mouth.side=1;
	d.r_mouth.rev=1;
	return d;
}
void doll_init(doll d)
{
	eye_init(d.l_eye);
	eye_init(d.r_eye);
	mouth_init(d.c_mouth);
	mouth_init(d.l_mouth);
	mouth_init(d.r_mouth);
	ear_init(d.l_ear);
	ear_init(d.r_ear);
	bow_init(d.r_bow);
	bow_init(d.l_bow);

	eye_set(d.l_eye);
	eye_set(d.r_eye);
	mouth_set(d.c_mouth);
	mouth_set(d.l_mouth);
	mouth_set(d.r_mouth);
	ear_set(d.l_ear);
	ear_set(d.r_ear);
	bow_set(d.r_bow);
	bow_set(d.l_bow);
}
void doll_set(doll d){
	eye_set(d.l_eye);
	eye_set(d.r_eye);
	mouth_set(d.c_mouth);
	mouth_set(d.l_mouth);
	mouth_set(d.r_mouth);
	ear_set(d.l_ear);
	ear_set(d.r_ear);
	bow_set(d.r_bow);
	bow_set(d.l_bow);
}

void eye_init(eye_d eye){
	ledcSetup(eye.channel_x,100,13);
	ledcSetup(eye.channel_y,100,13);
	ledcAttachPin(eye.channel_x_gpio,eye.channel_x);
	ledcAttachPin(eye.channel_y_gpio,eye.channel_y);
}
void mouth_init(mouth_d m)
{
	ledcSetup(m.channel,100,13);
	ledcAttachPin(m.channel_gpio,m.channel);
}
void ear_init(ear_d ear){
	ledcSetup(ear.channel,100,13);
	ledcAttachPin(ear.channel_gpio,ear.channel);
}
void bow_init(bow_d bow)
{
	ledcSetup(bow.channel_angle,100,13);
	ledcSetup(bow.channel_y,100,13);
	ledcAttachPin(bow.channel_angle_gpio,bow.channel_angle);
	ledcAttachPin(bow.channel_y_gpio,bow.channel_y);
}

void eye_set(eye_d eye){
	double x,y;


	if(eye.rev){
		x=eye.r*cos(radians(eye.angle))+1350;
		y=(eye.r)*sin(radians(eye.angle))*1.2+eye_center-50;
	}
	else{
		x=eye.r*cos(radians(eye.angle))+1500;
		y=(-eye.r)*sin(radians(eye.angle))*1.2+eye_center-130;
	}
	ledcWrite(eye.channel_y,(int)(y*0.8192));
	ledcWrite(eye.channel_x,(int)(x*0.8192));
}

void mouth_set(mouth_d m){
	if(!m.side){
		int duty = (1640-m.angle*92 /9)* 0.8192;
		ledcWrite(m.channel,duty);
	}
	else{
		if(m.rev){
			int duty = (1400-m.angle*92 /9)* 0.8192;
			ledcWrite(m.channel,duty);
		}else{
			int duty = (1520+m.angle*92 /9)* 0.8192;
			ledcWrite(m.channel,duty);
		}
	}
}

//ear angle is from 0 to 20
void ear_set(ear_d e){
	int duty;
	if(e.rev){
		duty =  (1650-e.angle*50/3)*0.8192;
	}
	else{
		duty =  (1350+e.angle*50/3)*0.8192;
	}
	ledcWrite(e.channel,duty);
}

void bow_set(bow_d b){
	int angle_duty,duty=0;

	if(b.rev){
		duty=(1500-b.y*50/3)*0.8192;
		angle_duty=(b.angle*50/3+1500)*0.8192;
	}
	else{
		duty=(b.y*50/3+1500-60)*0.8192;
		angle_duty=(1500-(b.angle-6)*50/3)*0.8192;
	}
	ledcWrite(b.channel_y,duty);
	ledcWrite(b.channel_angle,angle_duty);
}



