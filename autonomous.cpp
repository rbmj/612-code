/* states/state_autonomous.h
 *
 * Copyright (c) 2011, 2012 Chantilly Robotics <chantilly612@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * autonomous code
 */

#include <cmath>
#include "autonomous.h"
#include "state_tracker.h"
#include "ports.h"
#include "encoder.h"
#include "vision/vision_processing.h"

#include <vector>

state_tracker autonomous_substate;

void autonomous_substate_setup();
void autonomous_substate_launch();
void autonomous_substate_drive();

void autonomous_init() {
    // disable safety: we don't drive using RobotDrive
    drive.SetSafetyEnabled(false);
    shooter_turret.Shooter().disable();
    autonomous_substate.set_state(AUTNOM_STATE_SETUP);
    autonomous_substate.register_func(AUTNOM_STATE_SETUP,autonomous_substate_setup);
    autonomous_substate.register_func(AUTNOM_STATE_LAUNCH,autonomous_substate_launch);
    autonomous_substate.register_func(AUTNOM_STATE_DRIVE,autonomous_substate_drive);
}

void do_autonomous() {
    autonomous_substate.run_state();
}

void autonomous_substate_setup() {
    shooter_turret.Shooter().set_freq(SHOOT_FREQ);
    shooter_turret.Shooter().enable();
    shooter_turret.Winch().set_angle(LAUNCH_ANGLE_RAD);
    autonomous_substate.set_state(AUTNOM_STATE_LAUNCH);
    EncoderWheels::GetInstance().Disable();
}

void autonomous_substate_launch() {
    if(shooter_turret.Shooter().get_num_shot() < AUTNOM_NUM_BALLS) {
        if(shooter_turret.Shooter().at_setpoint()) {
            rollers.set_direction(roller_t::UP);
        }
        else {
            rollers.set_direction(roller_t::OFF);
        }
    }
    else {
        rollers.set_direction(roller_t::OFF);
        shooter_turret.Shooter().disable();
//        autonomous_substate.set_state(AUTNOM_STATE_DRIVE);
    }
}

void autonomous_substate_drive() {
    // todo
    //       N U M B E R S :
    //from alliance wall to closer side of middle bridge- 280 inches
    //from alliance wall to key- 144 inches
    //key is 48 inches deep
    //from end of key (closer to bridge wheels) to middle bridge is 88 inches?- drive a little less so we don't hit bridge.
    //couldn't find encoder's Pulses per Revolution - basing code on distance isntead
    //guess by Linda and Melissa:256
/*    for(bool far = true ; far ; ){
        //double distance = EncoderWheels::GetInstance().GetCurDistance();
        vector<double> distances = vision_processing::get_distance();
        double distance = -1;//actual distance should never be negative.
        for(unsigned int i=0; (i<distances.size()) && (distance <= 0); i++){
            if((distances[i] < 90)&& (distances[i] >0)){//within logical range, since key is 88 inches away and formula might not be completely exact
                distance = distances[i];//will break loop
            }
        }
        if(distance < 80){//80 feet will be good place to stop
            std::printf("current distance: %f", distance);
            drive.TankDrive(-.75 , -.75);//drive backwards at 3/4 full speed
        }
        else{//close to bridge
            far = false;//exit for loop, stop driving
        }
    }*/
    if(!EncoderWheels::GetInstance().IsEnabled()) {
        EncoderWheels::GetInstance().SetDistance(DISTANCE_TO_BRIDGE);
    }
}
