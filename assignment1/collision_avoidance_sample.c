#include <stdio.h>
#define BASE_SPEED_S 30
#define BASE_SPEED_M 70
#define SENSOR_P 1
#define MIN_DIST 30
#define SLOW_DIST 50

int ANGLE[2] = {-10,40};
int obsDistance[4] = {255,255,255,255};
int zone[4] = {2,2,2,2};  /*0 = < MIN_DIST, 1 = < SLOW_DIST, 2 = > SLOW_DIST*/
int motor_speed[2] = {0,0};

void determineSpeed(){
	//determine which zone the sensor is in
	for(int j=0;j<4;j++){
		if(obsDistance[j]<MIN_DIST){
			zone[j] = 0;
		}
		else if(obsDistance[j]<SLOW_DIST){
			zone[j] = 1;
		}
		else zone[j] = 2;
	}
	//both front sensors inside min range
	if(!zone[0] && !zone[1]){
		//if both side sensors inside min range, turn on spot
		if(!zone[2] && !zone[3]){
			motor_speed[0] = BASE_SPEED_S;
			motor_speed[1] = -BASE_SPEED_S;
		}
		//longer distance to right side, turn right and vice versa
		else{
			motor_speed[0] = (obsDistance[2] < obsDistance[3]) ? BASE_SPEED_S : -10;
			motor_speed[1] = (obsDistance[2] > obsDistance[3]) ? BASE_SPEED_S : -10;
		}
	}
	//if left front sensor is in min range but not front right, turn right
	else if(!zone[0] && zone[1]){
		motor_speed[0] = BASE_SPEED_S;
		motor_speed[1] = -10;
	}
	//if right front sensor is in min range but not front left, turn left
	else if(!zone[1] && zone[0]){
		motor_speed[1] = BASE_SPEED_S;
		motor_speed[0] = -10;
	}
	//if both front sensors dont see anything
	else if(zone[0] == 2 && zone[1] == 2){
		int s = (motor_speed[0] > motor_speed[1]) ? motor_speed[0] : motor_speed[1];
		motor_speed[0] = s;
		motor_speed[1] = s;
		//if one of the side sensors is inside min range
		if((!zone[2] || !zone[3]) && (zone[2] != zone[3])){
			//if left side sensor is inside min range, turn right and vice versa
			motor_speed[0] = (!zone[2]) ? BASE_SPEED_S : obsDistance[2]-20;
			motor_speed[1] = (!zone[3]) ? BASE_SPEED_S : obsDistance[3]-20;
		}
		//if nothing on the side increase speed towards BASE_SPEED_M
		else{
			if(motor_speed[0] < BASE_SPEED_M){
				motor_speed[0]+=0.5*(BASE_SPEED_M-motor_speed[0]);
				motor_speed[1]+=0.5*(BASE_SPEED_M-motor_speed[1]);
			}
			else{
				motor_speed[0] = BASE_SPEED_M;
				motor_speed[1] = BASE_SPEED_M;
			}
		}
	}
	else{
		//if both front sensors is in zone 1, the speed is decreased towards BASE_SPEED_S
		if(zone[0] == zone[1]){
			motor_speed[0] += 0.5*(BASE_SPEED_S-motor_speed[0]);
			motor_speed[1] += 0.5*(BASE_SPEED_S-motor_speed[1]);
		}
		//if only front right sensor is in zone 1, decrease speed and turn left
		else if(zone[1] == 1){
			motor_speed[1] += 3*(BASE_SPEED_S-motor_speed[1])/10;
			motor_speed[0] += 0.5*(BASE_SPEED_S-motor_speed[0]);
		}
		//if only front left sensor is in zone 1, decrease speed and turn right
		else{
			motor_speed[1] += 0.5*(BASE_SPEED_S-motor_speed[1]);
			motor_speed[0] += 3*(BASE_SPEED_S-motor_speed[0])/10;
		}
	}
}

int main() {
	obsDistance[0] = 58; obsDistance[1] = 31;
	determineSpeed();
    obsDistance[0] = 0; obsDistance[1] = 0;
    determineSpeed();
    obsDistance[0] = 0; obsDistance[1] = 0;
    determineSpeed();
	printf("%d, %d\n", motor_speed[0], motor_speed[1]); // 15, 7 (15, 9)
}

