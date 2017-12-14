/*
 ============================================================================
 Name        : Skrish_recruit.c
 Author      : Sai Krishnan
 Version     :
 Copyright   : Your copyright notice
 Description : Recruitment management tool, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *resume;   //Used to point to the individual resume's of all the candidates
char skill[20][128];  //Used to store all the skills from the skills.txt file
int totalSkills = 0; //Used to store the total number of skills in the skills.txt file
char tempResumeName[25];   //Used to read from candidates file
char resumeDetails[40];
char resumeFile[25]; //Used to store the complete path name of the resume files

/*Function to read all the skills from skills.txt and store into array 'skill' and also count total number of skills and store into 'totalSkills'*/
void readSkills() {
	FILE *skilList; //File pointer to point to skills.txt file
	if ((skilList = fopen("skills.txt", "r")) == NULL) {
		printf("Error! opening file");
		// Program exits if file pointer returns NULL.
		exit(1);
	}
	while (fgets(skill[totalSkills], sizeof(skill[totalSkills]), skilList)
			!= NULL) { /*Inputing the skill list from skills.txt file*/
		totalSkills++;
	}
	fclose(skilList);           //Closing pointer to skills file

	/*eliminate the newline character '\n' from all values in 'skill' array*/
	for (int j = 0; j < totalSkills; j++) {
		skill[j][strcspn(skill[j], "\n")] = 0;
	}
}

/*Function to calculate if a candidate is eligible and copies the resumes of eligible candidates into 'shortlist_<JD>_<DATE>.txt' file*/
void calcEligibility(int resumeSkill) {
	FILE *shortList;  //Used to point to 'shortlist_<JD>_<DATE>.txt'.
	char readCharacter;
	float percent = (resumeSkill * 100) / totalSkills; //Calculate percentage of skills
	if (percent > 50) { //If percent is greater than 50 the candidate is eligible
		if ((shortList = fopen("shortlists_softwareEngineer_29-10-17.txt", "a"))== NULL) {
			printf("Error opening the file");
			exit(1);
		}
		fseek(resume, 0, SEEK_SET);
		fseek(shortList, 0, SEEK_END);
		readCharacter = fgetc(resume);
		while (readCharacter != EOF) {
			fputc(readCharacter, shortList); //Storing the shortlisted resumes
			readCharacter = fgetc(resume);
		}
		fputs("\n\n\n\n", shortList);
	}
	fclose(shortList);
}


/*Function to extract the resume names from candidates.txt one by one and count the number of matching skills */
void processCandidates() {
	FILE *candidateList; //File pointer to point to candidates.txt
	int moveToSkills = 0; //Used to move to the 4th line in the resume file where the skills of the candidates are assumed start.
	if ((candidateList = fopen("candidates.txt", "r")) == NULL) {
		printf("Error! opening file");
		// Program exits if file pointer returns NULL.
		exit(1);
	}
	while (fgets(tempResumeName, sizeof(tempResumeName), candidateList) != NULL) { /* Starting the process of extraction from candidates one by one*/
		int resumeSkill = 0, checkSkill[totalSkills]; /*'resumeSkill' count number of matching skill.'checkSkill' keeps track of matched skill*/
		for (int i = 0; i < totalSkills; i++) {
			checkSkill[i] = 0;
		}
		strcpy(resumeFile, "ResumeBundle/");  //The path of the resume files
		tempResumeName[strcspn(tempResumeName, "\n")] = 0; /* Removing '\n' from the string*/
		strcat(resumeFile, tempResumeName); //Here the full path of individual resume files are stored in 'resumeFile'.

		if ((resume = fopen(resumeFile, "r")) == NULL) { //Points to individual candidate resume file
			printf("Error! opening file");
			// Program exits if file pointer returns NULL.
			exit(1);
		}
		while (fgets(resumeDetails, sizeof(resumeDetails), resume) != NULL) {
			if (moveToSkills++ >= 3) { //Ignoring name,qualification and experience sections of the resume.
				resumeDetails[strcspn(resumeDetails, "\n")] = 0; //Removing '\n' character from the end.
				for (int i = 0; i < totalSkills; i++) { //Logic to match the the skills of a candidate with the required skills to check eligibility
					if (strcmpi(resumeDetails, skill[i]) == 0) { //Compare ignoring case.Compare each candidate skill with the required skills.
						if (checkSkill[i] == 0) { //Check for repetition of skills in the candidate's resume so as to avoid discrepancy.
							checkSkill[i] = 1; //Checks the read skill so as not to repeat it.
							resumeSkill++;   //Increments the match count.
							break;
						} else
							break;
					}
				}
			}
		}
		calcEligibility(resumeSkill); //Function to calculate eligibility
	}
	fclose(candidateList);
	fclose(resume);
}
int main(void) {

	readSkills();  //Function to read from skills file
	processCandidates(); //Function to process and evaluate candidates from candidates.txt file
	return 0;
}
