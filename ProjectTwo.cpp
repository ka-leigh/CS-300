#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/* Kaleigh Pickle
CS 300 Project Two
Program for ABCU to load course data from a file and print course
information in a variety of ways
*/

// Course structure to hold all information about a course
struct Course {
	string courseId; //to hold course id
	string title; //to hold full title of course
	vector<string> prerequisites; // to hold any prerequisites for courses
};

//Binary Search Tree Node 
struct Node {
	Course course; // course data stored in node
	Node* left; // pointer to left child
	Node* right; // pointer to right child

	//constructor to initialize a node with a course
	Node(Course oneCourse) {
		course = oneCourse;
		left = nullptr;
		right = nullptr;
	}
};

// Binary Search Tree Class to store courses in osrted order
class BinarySearchTree {
private:
	Node* root; // pointer stored at root of node 

	// recursively add a node
	void addNode(Node* node, Course course) {
		if (course.courseId < node->course.courseId) { // go left
			if (node->left == nullptr)
				node->left = new Node(course); // add new node if left is empty
		else
			addNode(node->left, course);
		}
		else { // go right
			if (node->right == nullptr) 
				node->right = new Node(course); // add new node if right is empty
			else
				addNode(node->right, course);
		}
	}

	// in order traversal to print sorted cases 
	void inOrder(Node* node) {
		if (node != nullptr) {
			inOrder(node->left);
			cout << node->course.courseId << ", " << node->course.title << endl;
			inOrder(node->right);

		}
	}

	// recursive search for course by ID
	Course searchNode(Node* node, string courseId) {
		if (node == nullptr) {
			return Course(); // return empty if not found
		}
		if (node->course.courseId == courseId) {
			return node->course;
		}
		else if (courseId < node->course.courseId) {
			return searchNode(node->left, courseId);
		}
		else {
			return searchNode(node->right, courseId);
		}
	}

public:
	BinarySearchTree() { root = nullptr; }

	void Insert(Course course) {
		if (root == nullptr)
			root = new Node(course);
		else
			addNode(root, course);
	}

	void PrintInOrder() { inOrder(root); }

	Course Search(string courseId) { return searchNode(root, courseId); }
};

// Helper Functions
// split a line by a delimiter (comma) into tokens)
vector<string> SplitLine(const string& line, char delimiter) {
	vector<string> tokens;
	string token;
	stringstream ss(line);
	while (getline(ss, token, delimiter)) {
		if (!token.empty()) {
			tokens.push_back(token);
		}
	}
	return tokens;
}

// load courses from a file into the BST
void LoadCourses(string fileName, BinarySearchTree* bst) {
	ifstream file(fileName);
	if (!file.is_open()) {
		cout << "Error: Could not open file: " << fileName << endl;
		return;
	}

	string line;
	while (getline(file, line)) {
		vector<string> tokens = SplitLine(line, ',');
		if (tokens.size() < 2) continue; // skip invalid lines

		Course course;
		course.courseId = tokens[0];
		course.title = tokens[1];
		for (unsigned int i = 2; i < tokens.size(); ++i) {
			course.prerequisites.push_back(tokens[i]);
		}

		bst->Insert(course);
	}
	file.close();
	cout << "Courses loaded successfully!" << endl;
}

//print course info after searching by id
void PrintCourseInformation(BinarySearchTree* bst) {
	string courseId;
	cout << "Enter course ID: ";
	cin >> courseId;
	transform(courseId.begin(), courseId.end(), courseId.begin(), ::toupper);

	Course course = bst->Search(courseId);
	if (course.courseId.empty()) {
		cout << "Course not found." << endl;
		return;
	}

	cout << course.courseId << ", " << course.title << endl;
	if (course.prerequisites.empty()) {
		cout << "Prerequisites: None" << endl;
	}
	else {
		cout << "Prerequisites: ";
		for (unsigned int i = 0; i < course.prerequisites.size(); ++i) {
			cout << course.prerequisites[i];
			if (i < course.prerequisites.size() - 1)
				cout << ", ";
		}
		cout << endl;
	}
}

// Main Menu
int main() {
	BinarySearchTree* bst = new BinarySearchTree();
	string fileName;
	int choice = 0;

	cout << "Welcome to the ABCU Course Planner" << endl;
	cout << "----------------------------------------------" << endl;

	while (choice != 9) {
		cout << "\nMenu:" << endl;
		cout << " 1. Load Data Structure" << endl;
		cout << " 2. Print Course List" << endl;
		cout << " 3. Print Course Information" << endl;
		cout << " 9. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;


		switch (choice) {
		case 1: {
			string fileName = "C:\\Users\\kalei\\Documents\\ProjectTwo\\courses.csv"; // hardcoded path
			ifstream file(fileName);
			if (!file.is_open()) {
				cout << "Error: Could not open file at path: " << fileName << endl;
			}
			else {
				LoadCourses(fileName, bst);
			}
			break;
			}
			case 2:
				cout << "Here is a sample schedule: " << endl;
				bst->PrintInOrder();
				break;
			case 3:
				PrintCourseInformation(bst);
				break;
			case 9:
				cout << "Goodbye!" << endl;
				break;
			default:
				cout << "Invalid selection. Please choose from the menu options." << endl;

		}
	}

	return 0;
}