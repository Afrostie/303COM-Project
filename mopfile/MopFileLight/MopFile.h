#pragma once
/*e
 *This class manages the MOP files used by Moody
 */
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "MopState.h"
#include "MopItem.h"
#include "../../etc/Fragment.h"
#include "../../particle/normal/Particle.h"
#include <zlib.h>
class MopFile {
private:

    /**
     * the output file stream
     */
    std::ofstream outFileStream;

    /**
     * the input file steam
     */
    std::ifstream inFileStream;

    /**
     * the mop file name to be used
     */
    std::string filename;

    /**
     * Unspecified error flag
     *
     */

    int error;

    /**
     * get a single character from the input file stream
     */
    bool consumeChar(char &ch) {
        if (!this->inFileStream.eof()) {
            ch = this->inFileStream.get();
            return true;
        } else {
            return false;
        }
    }

    /**
     * read a MopState from the mop file and store it in a fragment
     * uses a std::string as an intermediate structure.
     *
     */
    bool readMopStateFromFileAsText(Fragment &input) {
        std::string tmp;
        char ch;
        bool result = false;
        do {
            result = this->consumeChar(ch);
            if (!result) return false;
            if(ch=='[') {
                this->error = 1;
                return false;
            }
            tmp.push_back(ch);
        } while (ch!='$');
        input.fill(tmp, tmp.length());
        return true;
    }

    /**
     * build a MopState from a fragment
     *
     */
    MopState  * buildMopStateFromFragment(Fragment &source) {
        MopState *ms = new MopState();
        Fragment worker;
        Fragment thing;
        int numParticles;
        int pos(0);
        int pos2(0);
        worker.fill(source,'|',pos);
        numParticles = worker.toInt();
        std::cout << "> Particle count " << numParticles << std::endl;
        for (int x(0);x<numParticles;x++) {
            MopItem mi;
            //pos++;
            worker.fill(source,'|',pos);
            std::cout << "> extracted string " << worker.c_str() << std::endl;
            pos2 = 0;
            thing.fill(worker,',',pos2);
            mi.id = thing.toInt();
            //std::cout << "> particle name: " << mi.name << std::endl;
            thing.fill(worker,',',pos2);
            mi.visualRepresentation = thing.toInt();
            thing.fill(worker,',',pos2);
            mi.red = thing.toInt();
            thing.fill(worker,',',pos2);
            mi.green = thing.toInt();
            thing.fill(worker,',',pos2);
            mi.blue = thing.toInt();
            thing.fill(worker,',',pos2);
            mi.x  = thing.toFloat();
              std::cout << "> extracted string " << thing.str() << std::endl;
          //std::cout << "X value: " << mi.x << std::endl;
            thing.fill(worker,',',pos2);
            mi.y = thing.toFloat();
            thing.fill(worker,',',pos2);
            mi.z  = thing.toFloat();
            ms->addMopItem(mi);
        }
        std::cout << "> converted the String to a MopState" << std::endl;
        return ms;
    }

    /**
     * if the filename given has no *.mop extension, add one.
     */
    std::string setMopFileExtension(std::string fn) {
        std::string temp;
        if(!(fn.substr(fn.find_last_of(".") + 1) == "mop")) {
            temp = fn.append(".mop");
        }
        return temp;
    }

    /**
     * close the mopfile reader
     */
    void closeMopfileReader() {
        this->inFileStream.close();
    }

public:
    MopFile() {
        this->error = 0;
    }
    ~MopFile() {
    }
    // no copy constructor for this class, there's no need to copy it anyway.

    /**
     * retrieve the filename
     */
    std::string getFilename() {
        return this->filename;
    }

    /**
     * read a single state from a mopfile (mopfile must already be open)
     */
    MopState * readState() {
        MopState * result = new MopState();
        //int itemCount;
        try {
            // start by reading in the next state from the file as a whole string
            Fragment initial;
            //std::cerr <<" > Reading the state as a Text String  " << std::endl;
            bool worked = this->readMopStateFromFileAsText(initial);
            std::cout <<initial.c_str()<<std::endl;

            if (!worked) {
                return NULL;
            }

            // now convert that to a MopState object
            //std::cerr <<" > starting string to mopstate conversion" << std::endl;
            result = this->buildMopStateFromFragment(initial);
            //std::cerr <<" >finishing string to mopstate conversion" << std::endl;
            if (!result) {
                return NULL;
            }
            if (this->error==1) {
                std::cerr <<"Error reading state from Mopfile" << std::endl;
                exit(0);
            }
        } catch(...) {
            std::cerr <<"Error reading state from Mopfile" << std::endl;
            exit(0);
        }
        return result;
    }
    /*
     * read a single state from the mop file, cycling back to the start of the file
     * if the end is reached
     */
    MopState * readCyclingState() {
        // get the next state from the mopfile
        MopState *incoming = this->readState();
        if (incoming == NULL) {
            this->resetFile();
            incoming = this->readState();
        }
        return incoming;
    }

    /**
     * This method sets the filename Mopfile will use. This varient prepends the path
     */
    void setFilename (std::string path,std::string fn){
        this->filename = path.append(this->setMopFileExtension(fn));
    };



    /**
     * This writes the current set of particles to the mopfile.
     * and sends it to be written to the specified file.
     * The data are stored as follows:
     * 1: An integer declaring how many particles are in the state
     * 2: A set of MopItems in text form, each holding the data for a particle
     * Note: the trailing comma is required, as reading the elements back correctly requires it
	 */
         /**

    void writeState(Particle *localSet, int environmentSetSize) {
        std::stringstream tmp;
        MopItem mi;

        tmp << environmentSetSize;
        for (int x(0); x<environmentSetSize; x++) {
            tmp << "|";
            mi = localSet[x].exportAsMopItem();
            tmp << mi.id << ",";
            tmp << mi.visualRepresentation << ",";
            tmp << mi.red << ",";
            tmp << mi.green << ",";
            tmp << mi.blue << ",";
            tmp << mi.x << ",";
            tmp << mi.y << ",";
            tmp << mi.z << ",";
        }
        std::string str = this->compress(tmp.str());
        this->outFileStream.open(this->filename.c_str(),std::ios::app);
        this->outFileStream << "@";
		this->outFileStream << str;
		this->outFileStream << "$";
        this->outFileStream.close();
    }
	*/

    void writeState(Particle *localSet, int environmentSetSize) {
        std::stringstream tmp;
        MopItem mi;
        tmp << environmentSetSize;
        for (int x(0);x<environmentSetSize;x++) {
            tmp << "|";
            mi = localSet[x].exportAsMopItem();
            tmp << mi.id << ",";
            tmp << mi.visualRepresentation << ",";
            tmp << mi.red << ",";
            tmp << mi.green << ",";
            tmp << mi.blue << ",";
            tmp << mi.x << ",";
            tmp << mi.y << ",";
            tmp << mi.z << ",";;
        }

        tmp << "$";
        std::string str = tmp.str();
        this->outFileStream.open(this->filename.c_str(),std::ios::app);
        this->outFileStream << str;
        this->outFileStream.close();
    }

    /**
     * close the mopfile writer
     */
    void closeMopfileWriter() {
        this->outFileStream.close();
    }
    /**
     * This method sets the filename Mopfile
     * This method sets the filename Mopfile will use. This varient does not prepend the path,
     * which assumes the path is already in the provided string, as would be the case for a file open dialog.
     * The mop extention is also not added.
     */
    void setFilename (std::string fn){
        this->filename = fn;
    };

    /**
     * open the mopfile for reading
     */
    void openMopfileReader() {
        this->closeMopfileReader();
        try {
            this->inFileStream.open(this->filename.c_str(),std::ios_base::in);
            this->inFileStream.seekg (0, std::ios_base::beg);
        } catch(...) {
            std::cerr << "error: cannot open mop file" << std::endl;
        }
    }

    /**
     * reset the mop file back to the start of the file
     */
    void resetFile() {
        this->closeMopfileReader();
        this->inFileStream.clear();
        this->openMopfileReader();
        this->inFileStream.seekg (0, std::ios_base::beg);
    }

};
