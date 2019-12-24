/*六足歩行型ロボットモーションプログラム
	・関数説明
	void set_mot_time(double time):モータ回転速度をtime[msec]に設定
	void set_mot_enable(int enable):モータのトルクを設定(enable 1:モータON 0:モータOFF）
	void leg_move(int leg_no,double x,double y, double z,double time)：leg_no(0~5)に該当する足先をx,y,zの位置にtime[msec]かけて移動させる
	void init_pose(double r,double z,double time):すべての足先を半径rの円上にtime[msec]かけて移動させる（ボディの高さをzの位置に移動させる）
	void end_pose(double time):time[msec]かけて待機ポーズを取る（すべての足を畳み込む）
	void greeting(double z,double r,double l,double time,double sita)
	・基本姿勢をinit(~)でとってから実行する
	・お辞儀をする（z：ボディの高さ　r：足先の位置の半径）
	・一連の動作を800等分し各ステップをsitaにて宣言することで使用する
	・各ステップの実行速度をtimeで宣言

	void saying(double z,double r,double l,double time,double sita)
	・基本姿勢をinit(~)でとってから実行する
	・話をしているように上をむく（z：ボディの高さ　r：足先の位置の半径）
	・一連の動作を800等分し各ステップをsitaにて宣言することで使用する
	・各ステップの実行速度をtimeで宣言

	void saying_roll(double z,double r,double l,double angle,double time,double sita)
	・基本姿勢をsaying(~)でとってから実行する
	・話をしているように上をむきながら左右に揺れる（z：ボディの高さ　r：足先の位置の半径）
	・一連の動作を800等分し各ステップをsitaにて宣言することで使用する
	・各ステップの実行速度をtimeで宣言

	void foot_step(double z, double r,double l,double time,double sita)
	・基本姿勢をinit(~)でとってから実行する
	・足踏みをする（z：ボディの高さ　r：足先の位置の半径 l:足を上げる高さ）
	・一連の動作を800等分し各ステップをsitaにて宣言することで使用する
	・各ステップの実行速度をtimeで宣言

	void body_wave(double z, double r,double l,double time,double angle)
	・基本姿勢をinit(~)でとってから実行する
	・体を波打つように動かす（z：ボディの高さ　r：足先の位置の半径 l:体を沈み込ませる高さ）
	・一連の動作を360等分し各ステップをangleにて宣言することで使用する
	・各ステップの実行速度をtimeで宣言

	void body_roll(double z, double r,double l,double time,double angle)
	・基本姿勢をinit(~)でとってから実行する
	・体自体を回転させるように移動させる（z：ボディの高さ　r：足先の位置の半径 l:移動量）
	・一連の動作を360等分し各ステップをangleにて宣言することで使用する
	・各ステップの実行速度をtimeで宣言
			 
	void walk_tri_pot_init(double z,double r,double time,double sita)
	・トライポット歩行をするための初期ポーズを取る（z：ボディの高さ　r：足先の位置の半径）
	・一連の動作を800等分し各ステップをsitaにて宣言することで使用する
	・各ステップの実行速度をtimeで宣言
			
	void walk_tri_pot(double z,double l,double r,double angle,double rotate,double time,double sita)
	・トライポット歩行を実行する（z：ボディの高さ　r：足先の位置の半径　l:移動量 rotate:回転量）
	・一連の動作を800等分し各ステップをsitaにて宣言することで使用する
	・各ステップの実行速度をtimeで宣言
		
	※追記事項
	・XYZ座標の原点は、ロボットの中心
	* timeの単位はmsec

 */

#include "include/motion.h"

motion_c::motion_c(){
		int i,j;	
		if (wiringPiSetupGpio() == -1) printf("IO Connect Error\n");
		rs405cb.connect(ENABLEPIN, BAUDRATE);
		
		set_mot_time(1);
}

void motion_c::get_sita(int leg_num,double x,double y,double z,double*sita){
	//sita1 -> theta1
	if(leg_num == 1 or leg_num == 4)sita[0] = atan2(y,x);
	else sita[0] = atan2(y,x) - ((45.0/180.0)*pi);
	
	//offset
	if (leg_num == 1 or leg_num == 4)
	{
		x -= 88.5 + L1*cos(atan2(y,x));
		y -= L1*sin(atan2(y,x));
	}
	else
	{
		x -= 62.5 + L1*cos(atan2(y,x));
		y -= 62.5 + L1*sin(atan2(y,x));
	}
	//convert rotate
	x = x/(cos(atan2(y,x)));
	//sita2
	sita[1] = atan2(z,x) - acos((-(L3*L3)+L2*L2+pow(x,2.0)+pow(z,2.0))/(2.0*L2*sqrt(pow(x,2.0)+pow(z,2.0))));
	//sita3
	sita[2] = -pi+acos((-pow(x,2.0)-pow(z,2.0)+L3*L3+L2*L2)/(2.0*L2*L3));
	
	//convert rad -> deg
	sita[0] *= 180.0/pi;
	sita[1] *= 180.0/pi;
	sita[2] *= 180.0/pi;
}

void motion_c::inverse_kinematics(){
	int i,j;
	double length;
	double pos_buf;
	double leg_pos_buf[6][3];

	//convert quadrante
	leg_pos_buf[0][0] =  leg_pos[0][1];
	leg_pos_buf[0][1] =  leg_pos[0][0];
	leg_pos_buf[0][2] =  leg_pos[0][2];
	
	leg_pos_buf[1][0] =  leg_pos[1][0];
	leg_pos_buf[1][1] = -leg_pos[1][1];
	leg_pos_buf[1][2] =  leg_pos[1][2];
	
	leg_pos_buf[2][0] =  leg_pos[2][0];
	leg_pos_buf[2][1] = -leg_pos[2][1];
	leg_pos_buf[2][2] =  leg_pos[2][2];
	
	leg_pos_buf[3][0] = -leg_pos[3][1];
	leg_pos_buf[3][1] = -leg_pos[3][0];
	leg_pos_buf[3][2] =  leg_pos[3][2];
	
	leg_pos_buf[4][0] = -leg_pos[4][0];
	leg_pos_buf[4][1] =  leg_pos[4][1];
	leg_pos_buf[4][2] =  leg_pos[4][2];
	
	leg_pos_buf[5][0] = -leg_pos[5][0];
	leg_pos_buf[5][1] =  leg_pos[5][1];
	leg_pos_buf[5][2] =  leg_pos[5][2];
	//get sita
	for(i=0;i<6;i++)
	{
		length = sqrt(pow(leg_pos_buf[i][0],2.0) + pow(leg_pos_buf[i][1],2.0) +
			      pow(leg_pos_buf[i][2],2.0));
		if(length < MAX_LENGTH)
		{
			get_sita(i,leg_pos_buf[i][0],leg_pos_buf[i][1],leg_pos_buf[i][2],motor_angle[i]);
		}
		else{
			printf("out of length \n");
		}
	}
	//set angle
	for(i=0;i<6;i++)
		for(j=0;j<3;j++)
			rs405cb.SetPosition(3*i+j+1,motor_angle[i][j]*10.0);
				
	//usleep(1000.0*10.0*motor_time);
}

void motion_c::leg_move(int leg_num,double x,double y,double z, double time){
	set_mot_time(time);
	
	leg_pos[leg_num][0] =  x;
	leg_pos[leg_num][1] =  y;
	leg_pos[leg_num][2] =  z;
	
	inverse_kinematics();
	
	usleep(1000.0*10.0*motor_time);
}

void motion_c::set_mot_time(double time){
	int i,j;
	
	motor_time = time;
		
	for(i=0;i<6;i++)
		for(j=0;j<3;j++)
			rs405cb.SetTime(3*i+j+1, motor_time);
}

void motion_c::set_mot_enable(int enable){
	int i,j;
	for(i=0;i<6;i++)
		for(j=0;j<3;j++)
			rs405cb.SetTorqueEnable(3*i+j+1, enable);
}

void motion_c::init_pose(double r,double z,double time){
	int i;
	set_mot_time(time);
	
	leg_pos[0][0] = r * sin(45.0*pi/180.0);
	leg_pos[1][0] = r * sin(90.0*pi/180.0);
	leg_pos[2][0] = r * sin(135.0*pi/180.0);
	leg_pos[3][0] = r * sin(-135.0*pi/180.0);
	leg_pos[4][0] = r * sin(-90.0*pi/180.0);
	leg_pos[5][0] = r * sin(-45.0*pi/180.0);
	
	leg_pos[0][1] = r * cos(45.0*pi/180.0);
	leg_pos[1][1] = r * cos(95.0*pi/180.0);
	leg_pos[2][1] = r * cos(135.0*pi/180.0);
	leg_pos[3][1] = r * cos(-135.0*pi/180.0);
	leg_pos[4][1] = r * cos(-90.0*pi/180.0);
	leg_pos[5][1] = r * cos(-45.0*pi/180.0);
	
	leg_pos[0][2] = z;
	leg_pos[1][2] = z;
	leg_pos[2][2] = z;
	leg_pos[3][2] = z;
	leg_pos[4][2] = z;
	leg_pos[5][2] = z;
	
	inverse_kinematics();
		
	usleep(1000.0*10.0*time);
}

void motion_c::end_pose(double time){
	double r = 250.0;
	double z = -25.0;
	
	set_mot_time(time);
	
	leg_pos[0][0] = 265 * sin(45.0*pi/180.0);
	leg_pos[1][0] = 250 * sin(90.0*pi/180.0);
	leg_pos[2][0] = 265 * sin(135.0*pi/180.0);
	leg_pos[3][0] = 265 * sin(-135.0*pi/180.0);
	leg_pos[4][0] = 250 * sin(-90.0*pi/180.0);
	leg_pos[5][0] = 265 * sin(-45.0*pi/180.0);
	
	leg_pos[0][1] = 265 * cos(45.0*pi/180.0);
	leg_pos[1][1] = 250 * cos(95.0*pi/180.0);
	leg_pos[2][1] = 265 * cos(135.0*pi/180.0);
	leg_pos[3][1] = 265 * cos(-135.0*pi/180.0);
	leg_pos[4][1] = 250 * cos(-90.0*pi/180.0);
	leg_pos[5][1] = 265 * cos(-45.0*pi/180.0);
		
	leg_pos[0][2] = z;
	leg_pos[1][2] = z;
	leg_pos[2][2] = z;
	leg_pos[3][2] = z;
	leg_pos[4][2] = z;
	leg_pos[5][2] = z;
	
	inverse_kinematics();
	
	usleep(1000.0*10.0*time*1.5);
}

void motion_c::greeting(double z,double r,double l,double time,double sita){
	sita = fmod(sita,800.0);
	set_mot_time(time);
	leg_pos[0][0] = r * sin(45.0*pi/180.0);
	leg_pos[1][0] = r * sin(90.0*pi/180.0);
	leg_pos[2][0] = r * sin(135.0*pi/180.0);
	leg_pos[3][0] = r * sin(-135.0*pi/180.0);
	leg_pos[4][0] = r * sin(-90.0*pi/180.0);
	leg_pos[5][0] = r * sin(-45.0*pi/180.0);
	
	leg_pos[0][1] = r * cos(45.0*pi/180.0);
	leg_pos[1][1] = r * cos(95.0*pi/180.0);
	leg_pos[2][1] = r * cos(135.0*pi/180.0);
	leg_pos[3][1] = r * cos(-135.0*pi/180.0);
	leg_pos[4][1] = r * cos(-90.0*pi/180.0);
	leg_pos[5][1] = r * cos(-45.0*pi/180.0);
	
	if(l > 0.0){		
		leg_pos[0][2] = z - (l     * fmod(sita,800.0)/800.0);
		leg_pos[1][2] = z - (l/2.0 * fmod(sita,800.0)/800.0);
		leg_pos[2][2] = z;
		leg_pos[3][2] = z;
		leg_pos[4][2] = z - (l/2.0 * fmod(sita,800.0)/800.0);
		leg_pos[5][2] = z - (l     * fmod(sita,800.0)/800.0);
	}
	else{
		leg_pos[0][2] = z + (l     * (1.0-fmod(sita,800.0)/800.0));
		leg_pos[1][2] = z + (l/2.0 * (1.0-fmod(sita,800.0)/800.0));
		leg_pos[2][2] = z;
		leg_pos[3][2] = z;
		leg_pos[4][2] = z + (l/2.0 * (1.0-fmod(sita,800.0)/800.0));
		leg_pos[5][2] = z + (l     * (1.0-fmod(sita,800.0)/800.0));
	}
	inverse_kinematics();
		
	usleep(1000.0*10.0*time);
}

void motion_c::saying(double z,double r,double l,double time,double sita){
	sita = fmod(sita,800.0);
	set_mot_time(time);
	leg_pos[0][0] = r * sin(45.0*pi/180.0);
	leg_pos[1][0] = r * sin(90.0*pi/180.0);
	leg_pos[2][0] = r * sin(135.0*pi/180.0);
	leg_pos[3][0] = r * sin(-135.0*pi/180.0);
	leg_pos[4][0] = r * sin(-90.0*pi/180.0);
	leg_pos[5][0] = r * sin(-45.0*pi/180.0);
	
	leg_pos[0][1] = r * cos(45.0*pi/180.0);
	leg_pos[1][1] = r * cos(95.0*pi/180.0);
	leg_pos[2][1] = r * cos(135.0*pi/180.0);
	leg_pos[3][1] = r * cos(-135.0*pi/180.0);
	leg_pos[4][1] = r * cos(-90.0*pi/180.0);
	leg_pos[5][1] = r * cos(-45.0*pi/180.0);
	
	if(l > 0.0){
		leg_pos[0][2] = z;
		leg_pos[1][2] = z - (l/2.0 * fmod(sita,800.0)/800.0);
		leg_pos[2][2] = z - (l     * fmod(sita,800.0)/800.0);
		leg_pos[3][2] = z - (l     * fmod(sita,800.0)/800.0);
		leg_pos[4][2] = z - (l/2.0 * fmod(sita,800.0)/800.0);
		leg_pos[5][2] = z;
	}
	else{
		leg_pos[0][2] = z;
		leg_pos[1][2] = z + (l/2.0 * (1.0-fmod(sita,800.0)/800.0));
		leg_pos[2][2] = z + (l     * (1.0-fmod(sita,800.0)/800.0));
		leg_pos[3][2] = z + (l     * (1.0-fmod(sita,800.0)/800.0));
		leg_pos[4][2] = z + (l/2.0 * (1.0-fmod(sita,800.0)/800.0));
		leg_pos[5][2] = z;
	}
	inverse_kinematics();
		
	usleep(1000.0*10.0*time);
}

void motion_c::saying_roll(double z,double r,double l,double angle,double time,double sita){
	sita = fmod(sita,800.0);
	set_mot_time(time);
	if(0.0 <= sita and sita < 400.0){
		leg_pos[0][0] = r * sin((45.0   + angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[1][0] = r * sin((90.0   + angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[2][0] = r * sin((135.0  + angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[3][0] = r * sin((-135.0 + angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[4][0] = r * sin((-90.0  + angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[5][0] = r * sin((-45.0  + angle*(fmod(sita,400)/400.0)) * pi/180.0);
		
		leg_pos[0][1] = r * cos((45.0   + angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[1][1] = r * cos((95.0   + angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[2][1] = r * cos((135.0  + angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[3][1] = r * cos((-135.0 + angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[4][1] = r * cos((-90.0  + angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[5][1] = r * cos((-45.0  + angle*(fmod(sita,400)/400.0)) * pi/180.0);
	}
	else if(400.0 <= sita and sita < 800.0){
		leg_pos[0][0] = r * sin((45.0   + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[1][0] = r * sin((90.0   + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[2][0] = r * sin((135.0  + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[3][0] = r * sin((-135.0 + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[4][0] = r * sin((-90.0  + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[5][0] = r * sin((-45.0  + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
		
		leg_pos[0][1] = r * cos((45.0   + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[1][1] = r * cos((95.0   + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[2][1] = r * cos((135.0  + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[3][1] = r * cos((-135.0 + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[4][1] = r * cos((-90.0  + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
		leg_pos[5][1] = r * cos((-45.0  + angle - angle*(fmod(sita,400)/400.0)) * pi/180.0);
	}	
	leg_pos[0][2] = z;
	leg_pos[1][2] = z - l/2.0;
	leg_pos[2][2] = z - l;
	leg_pos[3][2] = z - l;
	leg_pos[4][2] = z - l/2.0;
	leg_pos[5][2] = z;
	
	inverse_kinematics();
		
	usleep(1000.0*10.0*time);
}

void motion_c::foot_step(double z, double r,double l,double time,double sita){
	sita = fmod(sita,800.0);
	set_mot_time(time);
	leg_pos[0][0] = r * sin(45.0*pi/180.0);
	leg_pos[1][0] = r * sin(90.0*pi/180.0);
	leg_pos[2][0] = r * sin(135.0*pi/180.0);
	leg_pos[3][0] = r * sin(-135.0*pi/180.0);
	leg_pos[4][0] = r * sin(-90.0*pi/180.0);
	leg_pos[5][0] = r * sin(-45.0*pi/180.0);
	
	leg_pos[0][1] = r * cos(45.0*pi/180.0);
	leg_pos[1][1] = r * cos(95.0*pi/180.0);
	leg_pos[2][1] = r * cos(135.0*pi/180.0);
	leg_pos[3][1] = r * cos(-135.0*pi/180.0);
	leg_pos[4][1] = r * cos(-90.0*pi/180.0);
	leg_pos[5][1] = r * cos(-45.0*pi/180.0);
	
	if(0.0 <= sita and sita < 200.0){		
		leg_pos[0][2] = z - (l     * fmod(sita,200.0)/200.0);
		leg_pos[1][2] = z;
		leg_pos[2][2] = z - (l     * fmod(sita,200.0)/200.0);
		leg_pos[3][2] = z;
		leg_pos[4][2] = z - (l     * fmod(sita,200.0)/200.0); 
		leg_pos[5][2] = z;
	}
	else if(200.0 <= sita and sita < 400.0){
		leg_pos[0][2] = z - (l     * (1.0-fmod(sita,200.0)/200.0));
		leg_pos[1][2] = z;
		leg_pos[2][2] = z - (l     * (1.0-fmod(sita,200.0)/200.0));
		leg_pos[3][2] = z;
		leg_pos[4][2] = z - (l     * (1.0-fmod(sita,200.0)/200.0));
		leg_pos[5][2] = z;
	}
	else if(400.0 <= sita and sita < 600.0){
		leg_pos[0][2] = z;
		leg_pos[1][2] = z - (l     * fmod(sita,200.0)/200.0);
		leg_pos[2][2] = z;
		leg_pos[3][2] = z - (l     * fmod(sita,200.0)/200.0);
		leg_pos[4][2] = z; 
		leg_pos[5][2] = z - (l     * fmod(sita,200.0)/200.0);
	}
	else if(600.0 <= sita and sita < 800.0){
		leg_pos[0][2] = z;
		leg_pos[1][2] = z - (l     * (1.0-fmod(sita,200.0)/200.0));
		leg_pos[2][2] = z;
		leg_pos[3][2] = z - (l     * (1.0-fmod(sita,200.0)/200.0));
		leg_pos[4][2] = z;
		leg_pos[5][2] = z - (l     * (1.0-fmod(sita,200.0)/200.0));
	}
	inverse_kinematics();
		
	usleep(1000.0*10.0*time);
}

void motion_c::body_wave(double z, double r,double l,double time,double angle){
	set_mot_time(time);
	leg_pos[0][0] = r * sin(45.0*pi/180.0);
	leg_pos[1][0] = r * sin(90.0*pi/180.0);
	leg_pos[2][0] = r * sin(135.0*pi/180.0);
	leg_pos[3][0] = r * sin(-135.0*pi/180.0);
	leg_pos[4][0] = r * sin(-90.0*pi/180.0);
	leg_pos[5][0] = r * sin(-45.0*pi/180.0);
	
	leg_pos[0][1] = r * cos(45.0*pi/180.0);
	leg_pos[1][1] = r * cos(95.0*pi/180.0);
	leg_pos[2][1] = r * cos(135.0*pi/180.0);
	leg_pos[3][1] = r * cos(-135.0*pi/180.0);
	leg_pos[4][1] = r * cos(-90.0*pi/180.0);
	leg_pos[5][1] = r * cos(-45.0*pi/180.0);
			
	leg_pos[0][2] = z + l*sin(( 45.0 +angle) * pi/180.0);
	leg_pos[1][2] = z + l*sin(( 90.0 +angle) * pi/180.0);
	leg_pos[2][2] = z + l*sin(( 135.0+angle) * pi/180.0);
	leg_pos[3][2] = z + l*sin((-135.0+angle) * pi/180.0);
	leg_pos[4][2] = z + l*sin((-90.0 +angle) * pi/180.0);
	leg_pos[5][2] = z + l*sin((-45.0 +angle) * pi/180.0);

	inverse_kinematics();
		
	usleep(1000.0*10.0*time);
}

void motion_c::body_roll(double z, double r,double l,double time,double angle){
	set_mot_time(time);
	leg_pos[0][0] = r * sin(45.0*pi/180.0)   + l*sin(angle * pi/180.0);
	leg_pos[1][0] = r * sin(90.0*pi/180.0)   + l*sin(angle * pi/180.0);
	leg_pos[2][0] = r * sin(135.0*pi/180.0)  + l*sin(angle * pi/180.0);
	leg_pos[3][0] = r * sin(-135.0*pi/180.0) + l*sin(angle * pi/180.0);
	leg_pos[4][0] = r * sin(-90.0*pi/180.0)  + l*sin(angle * pi/180.0);
	leg_pos[5][0] = r * sin(-45.0*pi/180.0)  + l*sin(angle * pi/180.0);
	
	leg_pos[0][1] = r * cos(45.0*pi/180.0)   + l*cos(angle * pi/180.0);
	leg_pos[1][1] = r * cos(95.0*pi/180.0)   + l*cos(angle * pi/180.0);
	leg_pos[2][1] = r * cos(135.0*pi/180.0)  + l*cos(angle * pi/180.0);
	leg_pos[3][1] = r * cos(-135.0*pi/180.0) + l*cos(angle * pi/180.0);
	leg_pos[4][1] = r * cos(-90.0*pi/180.0)  + l*cos(angle * pi/180.0);
	leg_pos[5][1] = r * cos(-45.0*pi/180.0)  + l*cos(angle * pi/180.0);
			
	leg_pos[0][2] = z;
	leg_pos[1][2] = z;
	leg_pos[2][2] = z;
	leg_pos[3][2] = z;
	leg_pos[4][2] = z;
	leg_pos[5][2] = z;

	inverse_kinematics();
		
	usleep(1000.0*10.0*time);
}

void motion_c::walk_tri_pot_init(double z, double r,double time,double sita){
	sita = fmod(sita,800.0);	 
	set_mot_time(time);
	
	leg_pos[0][0] = r * sin(45.0*pi/180.0);
	leg_pos[1][0] = r * sin(90.0*pi/180.0);
	leg_pos[2][0] = r * sin(135.0*pi/180.0);
	leg_pos[3][0] = r * sin(-135.0*pi/180.0);
	leg_pos[4][0] = r * sin(-90.0*pi/180.0);
	leg_pos[5][0] = r * sin(-45.0*pi/180.0);
	
	leg_pos[0][1] = r * cos(45.0*pi/180.0);
	leg_pos[1][1] = r * cos(95.0*pi/180.0);
	leg_pos[2][1] = r * cos(135.0*pi/180.0);
	leg_pos[3][1] = r * cos(-135.0*pi/180.0);
	leg_pos[4][1] = r * cos(-90.0*pi/180.0);
	leg_pos[5][1] = r * cos(-45.0*pi/180.0);
	
	leg_pos[0][2] = z;
	leg_pos[1][2] = z - (z* fmod(sita,800.0)/800.0);
	leg_pos[2][2] = z;
	leg_pos[3][2] = z - (z* fmod(sita,800.0)/800.0);;
	leg_pos[4][2] = z;
	leg_pos[5][2] = z - (z* fmod(sita,800.0)/800.0);;
	
	inverse_kinematics();
	
	usleep(1000.0*10.0*time);
}

void motion_c::walk_tri_pot(double z,double l,double r,double angle,double rotate,double time,double sita){
	double rotate_b;
	sita = fmod(sita,800.0);
	set_mot_time(time);

	if(0.0 <= sita and sita < 200.0){
		rotate_b = rotate*(fmod(sita,200)/200.0);
		leg_pos[0][0] = r * sin( 45.0*pi/180.0)  - l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[1][0] = r * sin(( 90.0+rotate_b)*pi/180.0)  + l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[2][0] = r * sin( 135.0*pi/180.0) - l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[3][0] = r * sin((-135.0+rotate_b)*pi/180.0) + l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[4][0] = r * sin(-90.0*pi/180.0)  - l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[5][0] = r * sin((-45.0+rotate_b)*pi/180.0)  + l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;

		leg_pos[0][1] = r * cos( 45.0*pi/180.0)  - l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[1][1] = r * cos(( 90.0+rotate_b)*pi/180.0)  + l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[2][1] = r * cos( 135.0*pi/180.0) - l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[3][1] = r * cos((-135.0+rotate_b)*pi/180.0) + l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[4][1] = r * cos(-90.0*pi/180.0)  - l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[5][1] = r * cos((-45.0+rotate_b)*pi/180.0)  + l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;

		leg_pos[0][2] = z;
		leg_pos[1][2] = z * fmod(sita,200.0)/200.0;
		leg_pos[2][2] = z;
		leg_pos[3][2] = z * fmod(sita,200.0)/200.0;
		leg_pos[4][2] = z;
		leg_pos[5][2] = z * fmod(sita,200.0)/200.0;
	}
	else if(200.0 <= sita and sita < 400.0){
		rotate_b = rotate*(fmod(sita,200)/200.0);
		leg_pos[0][0] = r * sin(( 45.0+rotate_b)*pi/180.0)  - l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[1][0] = r * sin(( 90.0+rotate)*pi/180.0)  + l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[2][0] = r * sin(( 135.0+rotate_b)*pi/180.0) - l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[3][0] = r * sin((-135.0+rotate)*pi/180.0) + l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[4][0] = r * sin((-90.0+rotate_b)*pi/180.0)  - l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[5][0] = r * sin((-45.0+rotate)*pi/180.0)  + l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		
		leg_pos[0][1] = r * cos(( 45.0+rotate_b)*pi/180.0)  - l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[1][1] = r * cos(( 90.0+rotate)*pi/180.0)  + l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[2][1] = r * cos(( 135.0+rotate_b)*pi/180.0) - l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[3][1] = r * cos((-135.0+rotate)*pi/180.0) + l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[4][1] = r * cos((-90.0+rotate_b)*pi/180.0)  - l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[5][1] = r * cos((-45.0+rotate)*pi/180.0)  + l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		
		leg_pos[0][2] = z * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[1][2] = z;
		leg_pos[2][2] = z * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[3][2] = z;
		leg_pos[4][2] = z * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[5][2] = z;
	}
	else if(400.0 <= sita and sita < 600.0){
		rotate_b = rotate * (1.0 - fmod(sita,200)/200.0);
		leg_pos[0][0] = r * sin(( 45.0+rotate)*pi/180.0)  + l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[1][0] = r * sin(( 90.0+rotate_b)*pi/180.0)  - l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[2][0] = r * sin(( 135.0+rotate)*pi/180.0) + l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[3][0] = r * sin((-135.0+rotate_b)*pi/180.0) - l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[4][0] = r * sin((-90.0+rotate)*pi/180.0)  + l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[5][0] = r * sin((-45.0+rotate_b)*pi/180.0)  - l * sin((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		
		leg_pos[0][1] = r * cos(( 45.0+rotate)*pi/180.0)  + l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[1][1] = r * cos(( 90.0+rotate_b)*pi/180.0)  - l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[2][1] = r * cos(( 135.0+rotate)*pi/180.0) + l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[3][1] = r * cos((-135.0+rotate_b)*pi/180.0) - l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[4][1] = r * cos((-90.0+rotate)*pi/180.0)  + l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		leg_pos[5][1] = r * cos((-45.0+rotate_b)*pi/180.0)  - l * cos((angle/180.0) * pi) * fmod(sita,200.0)/200.0;
		
		leg_pos[0][2] = z * fmod(sita,200.0)/200.0;
		leg_pos[1][2] = z;
		leg_pos[2][2] = z * fmod(sita,200.0)/200.0;
		leg_pos[3][2] = z;
		leg_pos[4][2] = z * fmod(sita,200.0)/200.0;
		leg_pos[5][2] = z;
	}
	else if(600.0 <= sita and sita < 800.0){
		rotate_b = rotate * (1.0 - fmod(sita,200)/200.0);
		leg_pos[0][0] = r * sin(( 45.0+rotate_b)*pi/180.0)  + l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[1][0] = r * sin( 90.0*pi/180.0)  - l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[2][0] = r * sin(( 135.0+rotate_b)*pi/180.0) + l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[3][0] = r * sin(-135.0*pi/180.0) - l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[4][0] = r * sin((-90.0+rotate_b)*pi/180.0)  + l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[5][0] = r * sin(-45.0*pi/180.0)  - l * sin((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		
		leg_pos[0][1] = r * cos(( 45.0+rotate_b)*pi/180.0)  + l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[1][1] = r * cos( 90.0*pi/180.0)  - l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[2][1] = r * cos(( 135.0+rotate_b)*pi/180.0) + l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[3][1] = r * cos(-135.0*pi/180.0) - l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[4][1] = r * cos((-90.0+rotate_b)*pi/180.0)  + l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0); 
		leg_pos[5][1] = r * cos(-45.0*pi/180.0)  - l * cos((angle/180.0) * pi) * (1.0-fmod(sita,200.0)/200.0);
		
		leg_pos[0][2] = z;
		leg_pos[1][2] = z * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[2][2] = z; 
		leg_pos[3][2] = z * (1.0-fmod(sita,200.0)/200.0);
		leg_pos[4][2] = z;
		leg_pos[5][2] = z * (1.0-fmod(sita,200.0)/200.0);
	}
	inverse_kinematics();
}
