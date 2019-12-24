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

#ifndef MOTIONH
#define MOTIONH

#include <stdio.h>
#include <math.h>
#include "rs405cb.h"

class motion_c
{
private:
	double L1 = 90.0;
	double L2 = 120.0;
	double L3 = 170.0;
	double motor_time = 1.0;
	int pi = 3.141592;
	int BAUDRATE =  460800;
	int ENABLEPIN =  4;
	rs405cb_c rs405cb;
	void get_sita(int leg_num,double x,double y,double z,double*sita);
	void inverse_kinematics();
public:
	double leg_pos[6][3];
	double motor_angle[6][3];
	double MAX_LENGTH = L1+L2+L3;
	motion_c();
	void set_mot_time(double time);
	void set_mot_enable(int enable);
	void leg_move(int leg_no,double x,double y, double z,double time);
	void init_pose(double r,double z,double time);
	void end_pose(double time);
	void greeting(double z,double r,double l,double time,double sita);
	void saying(double z,double r,double l,double time,double sita);
	void saying_roll(double z,double r,double l,double angle,double time,double sita);
	void foot_step(double z, double r,double l,double time,double sita);
	void body_wave(double z, double r,double l,double time,double angle);
	void body_roll(double z, double r,double l,double time,double angle);
	void walk_tri_pot_init(double z,double r,double time,double sita);
	void walk_tri_pot(double z,double l,double r,double angle,double rotate,double time,double sita);
};

#endif // MOTIONH
