#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <fstream>
#include <time.h>
#include <vector>
#include <sstream>

using namespace std;

vector<string> errorArray;

// Binary Tree Node is the blueprint for a Node, defines strings, and the vector tells us the nodes children. (choices)
struct binaryTreeNode {
  string viewName;
  string viewLocation;
  string question;
  string response;
  vector<binaryTreeNode*> children;
  int replyNum;
};

// Calls errors from errors.txt when an invalid answer is given.
void populateErrors() {
	ifstream errors ("errors.txt");
	string temp;

if (errors.is_open())
{
		while (errors.eof() != true) {
			getline(errors, temp);
			errorArray.push_back(temp);
		}
	} else {
		cout << "Quit deleting important files." << endl;
	}

}

// randomizes error called.
int randomError () {
	int randomIndex;

	srand(time(NULL));

	randomIndex = rand() % (errorArray.size() - 1);

	return randomIndex;

}

// abbreviates name, location, questions, and responses, and inserts them into a node and and compares empty nodes to determine how many choices appear?
struct binaryTreeNode* createNode(string n, string l, string q, string r, int rn){
  if(n.compare("") == 0 && l.compare("") == 0 && q.compare("") == 0 && r.compare("") == 0){
    return NULL;
  }
  
  // Generate Node generates UI views and Nodes(Questions) to which we can respond to.
  binaryTreeNode* genNode = new binaryTreeNode;
  genNode->viewLocation = l;
  genNode->viewName = n;
  genNode->question = q;
  genNode->response = r;
  genNode->replyNum = rn;

  for(int i = 0; i < 6; i++) 
  {
    genNode->children.push_back(NULL);
    
  }

  return genNode;
}

//Source:https://www.studytonight.com/post/insertion-adding-a-new-node-in-a-binary-tree-data-structure

// Are our parameters out of order?
void insert(struct binaryTreeNode *root, string n, string l, string r, string ques, int rn) {
    struct binaryTreeNode *temp;
    queue<struct binaryTreeNode*>q;
    q.push(root);

    // If question and response are empty strings, the node is just a placeholder and should not be inserted
    if (ques.compare("") == 0 && r.compare("") == 0) {
      return;
    }

    while(!q.empty()) {
      temp = q.front();
      q.pop();

      if (temp->replyNum > 0){
        for(int i = 0; i < 6; i++)
        {
          if(temp->children[i] == NULL)
          {
            temp->children[i] = createNode(n, l, ques, r, rn);
            temp->replyNum--;
            return; 
          }
        }
      } else {
        for(int i = 0; i < 6; i++)
        {
          if(temp->children[i] != NULL){
            q.push(temp->children[i]);
          } else {
            break;
          }
        }
      }
    }



    // while(!q.empty())
    // {
    //     temp = q.front();
    //     q.pop();

    //     // Iterate through all six children; if a child is NULL, insert it there to complete the function. Otherwise, add it to the list of nodes to be explored if the node is not inserted in this level.
    //     for (int i = 0; i < 6; i++){
    //       if(temp->replyNum <= 0)
    //       {
    //         for (int j = 0; j < 6; j++) {
    //           if (temp->children[j] != NULL) {
    //             q.push(temp->children[j]);
    //           } else {
    //             break;
    //           }
    //         }
    //         break;

    //       } else if (temp->children[i] == NULL){
    //         if (n.compare("") && l.compare("") == 0 && r.compare("") == 0 && ques.compare("") == 0){
    //           temp->children[i] = NULL;
    //         } else {
    //           temp->children[i] = createNode(n, l, ques, r, rn);
    //           temp->replyNum -=1;
    //         }
    //         break;
    //       }
    //     }
    // }
    // return;
}

// wraps the line after 80 character limit is hit.
void wrapLine (string& text){
    int len = text.size();
    int lineTracker = 0;

    for (int i = 0; i < len; i++){

        if(lineTracker % 80 == 0 && text[i] == 32) {
          continue;
        }
        cout << text[i];
        lineTracker++;

        if(lineTracker % 80 == 0) {

            if (text[i] != 32 && text[i+1] != 32){
                int j = i + 1;
                while(text[j] != 32){
                    cout << text[j];
                    j++;
                    i++;
                }
            }
            cout << endl;
        }
    }
}

// Generates a larger majority of UI elements and formatting.
binaryTreeNode* nodePrompter (binaryTreeNode* cur) {
	int ans = 0;
  bool validNode = false;

  // If all the children of the current node are null, we are at the end of the dialogue,
  // so just return and end program execution
  for (int i = 0; i < 6; i++){
    if (cur->children[i] != NULL) {
      validNode = true;
      break;
    }
  }

  if (validNode == false){
    return NULL;
  }

  for(int i = 0; i < 80; i++) {
    cout << "-";
  }
  
  cout << endl;

  cout << cur->viewLocation;
  cout << " - ";
  cout << cur->viewName;
  cout << endl;
  for(int i = 0; i < 80; i++) {
    cout << "-";
  }
  cout << endl << endl;
  wrapLine(cur->question);
  cout << endl;

  cout << "\n\tYour Response? - ";
  cout << endl << endl;

  // Print out all possible answers to stdout
  for(int i = 0; i < 6; i++) {
    if(cur->children[i] != NULL) {
      cout << "\t\t" << i+1 << "- ";
      wrapLine(cur->children[i]->response);
      cout << endl;
    }
  }
  
  // Prompt for and get user answer
  cout << endl << endl << "> ";

  cin >> ans;

  cout << endl;

  bool properInput = false;
  // Check which answer the user input and direct the dialogue path to that answer
  for(int i = 1; i <= 6; i++) {
    if (ans == i) {
      cur = cur->children[i-1];
      properInput = true;
      break;
    }
  }

  if (properInput == false) {
    cout << errorArray[randomError()] << endl << endl;
  }

  return cur;
}

// Checks for NULL line using "~"
string interpretLine(ifstream & file){
  string temp;

  getline(file, temp);

  if(temp.compare("~") == 0){
    temp = "";
  }
  return temp;
}

// Some UI related but there's a lot more to explain in here, most if not all of the code is tied to this function.
int main() {
	ifstream dialogue ("dialogue-input.txt");
	string tempName;
  string tempLocation;
  string tempResponse;
	string tempQuestion;
  string tempReplyNumS;
  int tempReplyNum = 0;
	binaryTreeNode* root;

	populateErrors();

	if (dialogue.is_open()) {
    dialogue.ignore(1000, '\n');
    tempResponse = interpretLine(dialogue);
    tempLocation = interpretLine(dialogue);
    tempName = interpretLine(dialogue);
    tempQuestion = interpretLine(dialogue);
    getline(dialogue, tempReplyNumS);
    if(tempReplyNumS.compare("~") == 0)
    {
      tempReplyNum = 0;
    } else {
      stringstream replyStream(tempReplyNumS);
      replyStream >> tempReplyNum;
    }

    root = createNode(tempName, tempLocation, tempQuestion, tempResponse, tempReplyNum);


		while (dialogue.eof() != true) {
      dialogue.ignore(1000, '\n');
			tempResponse = interpretLine(dialogue);
      if (tempResponse.compare("") == 0) {
        tempLocation = "";
        tempName = "";
        tempQuestion = "";
        tempReplyNum = 0;
      } else {
        tempLocation = interpretLine(dialogue);
        tempName = interpretLine(dialogue);
			  tempQuestion = interpretLine(dialogue);
        getline(dialogue, tempReplyNumS);
        if (tempReplyNumS.compare("~") == 0) {
          tempReplyNum = 0;
        } else {
          stringstream replyStream(tempReplyNumS);
          replyStream >> tempReplyNum;
        }
      }

      insert(root, tempName, tempLocation, tempResponse, tempQuestion, tempReplyNum);

		}
	} else {
		cout << "Quit deleting important files." << endl;
	}


	binaryTreeNode* current = root;

	while (current != NULL) {
		current = nodePrompter(current);
	}

	return 0;

}
