/*
 *	wiiuse
 *
 *	Written By:
 *		Michael Laforest	< para >
 *		Email: < thepara (--AT--) g m a i l [--DOT--] com >
 *
 *	Copyright 2006-2007
 *
 *	This file is part of wiiuse.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	$Header$
 *
 */

/**
 *	@file
 *
 *	@brief Example using the wiiuse API.
 *
 *	This file is an example of how to use the wiiuse library.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <fstream>

#include <cstdlib>
#include <ctime>
#include <string>

#include "hmm.h"
#include "sample_3d.h"
#include "gesture.h"
#include "GestureModel.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#ifndef WIN32
	#include <unistd.h>
#endif

#include "wiiuse.h"


#define MAX_WIIMOTES    1

std::vector<Gesture> importDataFile(char* filename){
    int module = 3; //i valori sono a 3 a 3.

    std::ifstream train_fs(filename);

    std::string word; // parola correntemente acquisita

    std::vector<Gesture> dataset; // dataset di gesture (non discrete) da ricostruire

    int count = 0;// n° di gesture correntemente processate

    int n_word = 0;// n° dati acquisiti per la gesture corrente

    double temp[module];// terna di accelerazioni corrente

    while(!train_fs.eof()){

        // acquisisce la prima parola FORMATTATA
        train_fs >> word;

        // inizio di una gesture
        if(word == "<gesture>"){

            // crea una nuova gesture nel dataset
            dataset.push_back(Gesture());

            // incrementa il n° di gesture processate
            count++;

        }
        // fase acquisizione dati della gesture
        else if(word != "<gesture>" && word != "</gesture>"){

            // converte la parola acquisita in mumero
            std::istringstream num(word);
            double value;
            num >> value;

            // memorizza il numero
            temp[n_word%module] = value;

            // se ho memorizzato una terna di numeri, crea da essi un Sample_3d
            // e inseriscilo nella gesture corrente
            if( (n_word % module) == 2){
                //temp[2] = temp[2] - 1;
                dataset.at(count-1).add(Sample_3d(temp));
            }

            // incrementa il contatore delle parole processate
            n_word++;

        }
        // fine di una gesture
        else if(word == "</gesture>"){

            n_word = 0;

        }

    }

return dataset;
}

std::vector<Gesture> importDataString( std::stringstream train_fs ){
    int module = 3; //i valori sono a 3 a 3.

    std::string word; // parola correntemente acquisita

    std::vector<Gesture> dataset; // dataset di gesture (non discrete) da ricostruire

    int count = 0;// n° di gesture correntemente processate

    int n_word = 0;// n° dati acquisiti per la gesture corrente

    double temp[module];// terna di accelerazioni corrente

    while(!train_fs.eof()){

        // acquisisce la prima parola FORMATTATA
        train_fs >> word;

        // inizio di una gesture
        if(word == "<gesture>"){

            // crea una nuova gesture nel dataset
            dataset.push_back(Gesture());

            // incrementa il n° di gesture processate
            count++;

        }
        // fase acquisizione dati della gesture
        else if(word != "<gesture>" && word != "</gesture>"){

            // converte la parola acquisita in mumero
            std::istringstream num(word);
            double value;
            num >> value;

            // memorizza il numero
            temp[n_word%module] = value;

            // se ho memorizzato una terna di numeri, crea da essi un Sample_3d
            // e inseriscilo nella gesture corrente
            if( (n_word % module) == 2){
                //temp[2] = temp[2] - 1;
                dataset.at(count-1).add(Sample_3d(temp));
            }

            // incrementa il contatore delle parole processate
            n_word++;

        }
        // fine di una gesture
        else if(word == "</gesture>"){

            n_word = 0;

        }

    }

return dataset;
}

/**
 *	@brief Callback that handles an event.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *
 *	This function is called automatically by the wiiuse library when an
 *	event occurs on the specified wiimote.
 */
void handle_event(struct wiimote_t* wm, std::stringstream* file, bool& endGesture) {

    if (wm->btns) {
		/*
		 *	Pressing minus will tell the wiimote we are no longer interested in movement.
		 *	This is useful because it saves battery power.
		 */
		if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS)){
            printf("\n\n--- EVENT ACC_OFF [wiimote id %i] ---\n", wm->unid);
			wiiuse_motion_sensing(wm, 0);
		}

		/*
		 *	Pressing plus will tell the wiimote we are interested in movement.
		 */
		if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS)){
            printf("\n\n--- EVENT ACC_ON [wiimote id %i] ---\n", wm->unid);
			wiiuse_motion_sensing(wm, 1);
		}
    }

    /* if the accelerometer is turned on then print angles */
	if (WIIUSE_USING_ACC(wm)) {
		if(wm->btns){
		    if(IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_A)){
                //printf("INIZIO GESTURE\n");
                *file << "<gesture>" << std::endl;
		    }
		}

        if(wm->btns_held){
		    if(IS_HELD(wm, WIIMOTE_BUTTON_A)){
                //printf("wiimote X acc   = %f\n", wm->gforce.x);
                //printf("wiimote Y acc   = %f\n", wm->gforce.y);
                //printf("wiimote Z acc   = %f\n", wm->gforce.z);

                *file << wm->gforce.x << " " << wm->gforce.y << " " << wm->gforce.z << std::endl;
		    }
        }

        if(wm->btns_released){
            if(IS_RELEASED(wm, WIIMOTE_BUTTON_A)){
                //printf("FINE GESTURE\n");
                *file << "</gesture>" << std::endl;
                endGesture = true;
		    }
		}
	}
}


/**
 *	@brief Callback that handles a read event.
 *
 *	@param wm		Pointer to a wiimote_t structure.
 *	@param data		Pointer to the filled data block.
 *	@param len		Length in bytes of the data block.
 *
 *	This function is called automatically by the wiiuse library when
 *	the wiimote has returned the full data requested by a previous
 *	call to wiiuse_read_data().
 *
 *	You can read data on the wiimote, such as Mii data, if
 *	you know the offset address and the length.
 *
 *	The \a data pointer was specified on the call to wiiuse_read_data().
 *	At the time of this function being called, it is not safe to deallocate
 *	this buffer.
 */
void handle_read(struct wiimote_t* wm, byte* data, unsigned short len) {
	int i = 0;

	printf("\n\n--- DATA READ [wiimote id %i] ---\n", wm->unid);
	printf("finished read of size %i\n", len);
	for (; i < len; ++i) {
		if (!(i%16))
			printf("\n");
		printf("%x ", data[i]);
	}
	printf("\n\n");
}


/**
 *	@brief Callback that handles a controller status event.
 *
 *	@param wm				Pointer to a wiimote_t structure.
 *	@param attachment		Is there an attachment? (1 for yes, 0 for no)
 *	@param speaker			Is the speaker enabled? (1 for yes, 0 for no)
 *	@param ir				Is the IR support enabled? (1 for yes, 0 for no)
 *	@param led				What LEDs are lit.
 *	@param battery_level	Battery level, between 0.0 (0%) and 1.0 (100%).
 *
 *	This occurs when either the controller status changed
 *	or the controller status was requested explicitly by
 *	wiiuse_status().
 *
 *	One reason the status can change is if the nunchuk was
 *	inserted or removed from the expansion port.
 */
void handle_ctrl_status(struct wiimote_t* wm) {
	printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", wm->unid);

	printf("attachment:      %i\n", wm->exp.type);
	printf("speaker:         %i\n", WIIUSE_USING_SPEAKER(wm));
	printf("ir:              %i\n", WIIUSE_USING_IR(wm));
	printf("leds:            %i %i %i %i\n", WIIUSE_IS_LED_SET(wm, 1), WIIUSE_IS_LED_SET(wm, 2), WIIUSE_IS_LED_SET(wm, 3), WIIUSE_IS_LED_SET(wm, 4));
	printf("battery:         %f %%\n", wm->battery_level);
}


/**
 *	@brief Callback that handles a disconnection event.
 *
 *	@param wm				Pointer to a wiimote_t structure.
 *
 *	This can happen if the POWER button is pressed, or
 *	if the connection is interrupted.
 */
void handle_disconnect(wiimote* wm) {
	printf("\n\n--- DISCONNECTED [wiimote id %i] ---\n", wm->unid);
}


void test(struct wiimote_t* wm, byte* data, unsigned short len) {
	printf("test: %i [%x %x %x %x]\n", len, data[0], data[1], data[2], data[3]);
}



/**
 *	@brief main()
 *
 *	Connect to up to two wiimotes and print any events
 *	that occur on either device.
 */
int main(int argc, char** argv) {

    //parsing degli input
    if(argc < 6){
        std::cout<<"USO: wiiGesture <train_file1> <train_file2> <train_file3> <num_stati> <span>"<<std::endl;
        return 1;
    }

	wiimote** wiimotes;
	int found, connected;

	/*
	 *	Initialize an array of wiimote objects.
	 *
	 *	The parameter is the number of wiimotes I want to create.
	 */
	wiimotes =  wiiuse_init(MAX_WIIMOTES);

	/*
	 *	Find wiimote devices
	 *
	 *	Now we need to find some wiimotes.
	 *	Give the function the wiimote array we created, and tell it there
	 *	are MAX_WIIMOTES wiimotes we are interested in.
	 *
	 *	Set the timeout to be 5 seconds.
	 *
	 *	This will return the number of actual wiimotes that are in discovery mode.
	 */
	found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
	if (!found) {
		printf ("No wiimotes found.");
		return 0;
	}

	/*
	 *	Connect to the wiimotes
	 *
	 *	Now that we found some wiimotes, connect to them.
	 *	Give the function the wiimote array and the number
	 *	of wiimote devices we found.
	 *
	 *	This will return the number of established connections to the found wiimotes.
	 */
	connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
	if (connected)
		printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	else {
		printf("Failed to connect to any wiimote.\n");
		return 0;
	}

	/*
	 *	Now set the LEDs and rumble for a second so it's easy
	 *	to tell which wiimotes are connected (just like the wii does).
	 */
	wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
	//wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
	//wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
	//wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);
	//wiiuse_set_accel_threshold(wiimotes[0], 1);
	wiiuse_rumble(wiimotes[0], 1);
	//wiiuse_rumble(wiimotes[1], 1);

	#ifndef WIN32
		usleep(200000);
	#else
		Sleep(200);
	#endif

	wiiuse_rumble(wiimotes[0], 0);
	//wiiuse_rumble(wiimotes[1], 0);

    std::stringstream buffer;

    char* train1 = argv[1];
    char* train2 = argv[2];
    char* train3 = argv[3];

    std::istringstream s_stati(argv[4]);
    int stati;
    s_stati >> stati;

    std::istringstream s_span(argv[5]);
    int span;
    s_span >> span;

    std::cout<<"numero stati: "<<stati<<" | span: "<<span<<std::endl;

    //Costruzione dei Trainset
    std::vector<Gesture> trainset1 = importDataFile(train1);
    std::vector<Gesture> trainset2 = importDataFile(train2);
    std::vector<Gesture> trainset3 = importDataFile(train3);

    //Costruzione dei GesturModel
    GestureModel* gesture1;
    gesture1 = new GestureModel(stati, span);

    GestureModel* gesture2;
    gesture2 = new GestureModel(stati, span);

    GestureModel* gesture3;
    gesture3 = new GestureModel(stati, span);

    //Addestramento dei quantizzatore
    gesture1->trainQuantizer(trainset1);
    gesture2->trainQuantizer(trainset2);
    gesture3->trainQuantizer(trainset3);

    std::cout<<"quantizerS: OK"<<std::endl;

    //Addestramento HMMs
    gesture1->trainHMM(trainset1);
    gesture2->trainHMM(trainset2);
    gesture3->trainHMM(trainset3);

    std::cout<<"hmmS: TRAINED"<<std::endl;



    bool one_wiimote_connected = true;
    bool finishedGesture = false;
	/*
	 *	Maybe I'm interested in the battery power of the 0th
	 *	wiimote.  This should be WIIMOTE_ID_1 but to be sure
	 *	you can get the wiimote assoicated with WIIMOTE_ID_1
	 *	using the wiiuse_get_by_id() function.
	 *
	 *	A status request will return other things too, like
	 *	if any expansions are plugged into the wiimote or
	 *	what LEDs are lit.
	 */
	//wiiuse_status(wiimotes[0]);

	/*
	 *	This is the main loop
	 *
	 *	wiiuse_poll() needs to be called with the wiimote array
	 *	and the number of wiimote structures in that array
	 *	(it doesn't matter if some of those wiimotes are not used
	 *	or are not connected).
	 *
	 *	This function will set the event flag for each wiimote
	 *	when the wiimote has things to report.
	 */
	while (1 && one_wiimote_connected) {
		if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
			/*
			 *	This happens if something happened on any wiimote.
			 *	So go through each one and check if anything happened.
			 */
			int i = 0;
			for (; i < MAX_WIIMOTES; ++i) {
				switch (wiimotes[i]->event) {
					case WIIUSE_EVENT:
						/* a generic event occured */
						handle_event(wiimotes[i], &buffer, finishedGesture);
						if( finishedGesture ){
                            std::ofstream tempFile("tempGesture.txt", std::ios_base::trunc);
                            //std::ifstream tempFile("tempGesture");
                            tempFile<<buffer.str();
                            std::vector<Gesture> currentGestur =  importDataFile( "tempGesture.txt" );

                            std::vector<double> prob1 = gesture1->evaluateGestures( currentGestur );
                            std::vector<double> prob2 = gesture2->evaluateGestures( currentGestur );
                            std::vector<double> prob3 = gesture3->evaluateGestures( currentGestur );

                            for(int i=0; i<prob1.size(); i++){
                                double a,b,c;
                                a=prob1.at(i);
                                b=prob2.at(i);
                                c=prob3.at(i);

                                double max = a;
                                int max_id = 1;
                                if(b>max){
                                    max = b;
                                    max_id = 2;
                                }
                                if(c>max){
                                    max = c;
                                    max_id = 3;
                                }

                                if(max < 0.0000000001){
                                    max_id =0;
                                }

                                if(max_id == 1)
                                    std::cout<< "QUADRATO" <<std::endl;
                                if(max_id == 2)
                                    std::cout<< "TRIANGOLO" <<std::endl;
                                if(max_id == 3)
                                    std::cout<< "ZETA" <<std::endl;
                                if(max_id == 0)
                                    std::cout<< "GESTURE NON RICONOSCIUTA!!!" <<std::endl;
                            }
                            finishedGesture = false;
                            tempFile.close();
                            buffer.str("");
						}
						break;

					case WIIUSE_STATUS:
						/* a status event occured */
						handle_ctrl_status(wiimotes[i]);
						break;

					case WIIUSE_DISCONNECT:
					case WIIUSE_UNEXPECTED_DISCONNECT:
						/* the wiimote disconnected */
						handle_disconnect(wiimotes[i]);
						one_wiimote_connected = false;
						break;

					case WIIUSE_READ_DATA:
						/*
						 *	Data we requested to read was returned.
						 *	Take a look at wiimotes[i]->read_req
						 *	for the data.
						 */
						break;

					case WIIUSE_NUNCHUK_INSERTED:
						/*
						 *	a nunchuk was inserted
						 *	This is a good place to set any nunchuk specific
						 *	threshold values.  By default they are the same
						 *	as the wiimote.
						 */
						 //wiiuse_set_nunchuk_orient_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 90.0f);
						 //wiiuse_set_nunchuk_accel_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 100);
						printf("Nunchuk inserted.\n");
						break;

					case WIIUSE_CLASSIC_CTRL_INSERTED:
						printf("Classic controller inserted.\n");
						break;

					case WIIUSE_GUITAR_HERO_3_CTRL_INSERTED:
						/* some expansion was inserted */
						handle_ctrl_status(wiimotes[i]);
						printf("Guitar Hero 3 controller inserted.\n");
						break;

					case WIIUSE_NUNCHUK_REMOVED:
					case WIIUSE_CLASSIC_CTRL_REMOVED:
					case WIIUSE_GUITAR_HERO_3_CTRL_REMOVED:
						/* some expansion was removed */
						handle_ctrl_status(wiimotes[i]);
						printf("An expansion was removed.\n");
						break;

					default:
						break;
				}
			}
		}
	}

	/*
	 *	Disconnect the wiimotes
	 */
	wiiuse_cleanup(wiimotes, MAX_WIIMOTES);

	return 0;
}

