#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <set>
#define RIGHT 0
#define LEFT 1
#define DOWN 2
#define UP 3
#define NPUZZLE_NODE_H
#define BOARD_SQ_SIZE 3
#define PRINT_W 3

typedef int direction_t;
typedef int8_t puzzle_t;
typedef int cost_t;
typedef int parent_t;
#ifndef NPUZZLE_ASTARSEARCH_H
#define NPUZZLE_ASTARSEARCH_H

#define MANHATTAN_DISTANCE 1
#define HAMMING_DISTANCE 2
#define LINEAR_CONFLICT 3

#define LIMIT_DEPTH 60
#define NODE_LIMIT 10000000

#define cost_ cost
#define parent_ parent


using namespace std;

int dirX[4] = {0, 0, 1, -1}; // RIGHT-LEFT-DOWN-UP
int dirY[4] = {1, -1, 0, 0}; // RIGHT-LEFT-DOWN-UP


#define MAX 105
class Node {
public:
	puzzle_t **A = nullptr;
	bool emptyNode = true;
	static int boardSqSize;

	friend ostream &operator<<(ostream &os, const Node &node);

	Node() {
		emptyNode = true;
		A = new puzzle_t *[boardSqSize];
		for (int i = 0; i < boardSqSize; ++i) {
			A[i] = new puzzle_t[boardSqSize];
			memset(A[i], 0, boardSqSize * sizeof(A[0][0]));
		}
	}


	Node(const Node &node) {
		this->~Node();
		this->emptyNode = node.emptyNode;
		A = new puzzle_t *[boardSqSize];
		for (int i = 0; i < boardSqSize; ++i) {
			A[i] = new puzzle_t[boardSqSize];
		}
		for (int i = 0; i < boardSqSize; i++) {
			for (int j = 0; j < boardSqSize; j++) {
				A[i][j] = node.A[i][j];
			}
		}
	}

	Node &operator=(const Node &node) {
		this->~Node();
		this->emptyNode = node.emptyNode;
		A = new puzzle_t *[boardSqSize];
		for (int i = 0; i < boardSqSize; ++i) {
			A[i] = new puzzle_t[boardSqSize];
		}
		for (int i = 0; i < boardSqSize; i++) {
			for (int j = 0; j < boardSqSize; j++) {
				A[i][j] = node.A[i][j];
			}
		}
		return *this;
	}

	~Node() {
		if (A == nullptr) return;
		for (int i = 0; i < boardSqSize; ++i) {
			delete A[i];
		}
		delete[] A;
		A = nullptr;
	}

	bool operator==(const Node &right) const {
		for (int i = 0; i < boardSqSize; i++)
			for (int j = 0; j < boardSqSize; j++)
				if (A[i][j] != right.A[i][j]) return false;
		return true;
	}

	bool operator!=(const Node &right) const {
		return !(*this == right);
	}

	bool operator<(const Node &right) const {
		for (int i = 0; i < boardSqSize; i++) {
			for (int j = 0; j < boardSqSize; j++) {
				if (A[i][j] < right.A[i][j]) return true;
				else if (A[i][j] == right.A[i][j]) continue;
				else return false;
			}
		}
		return false;
	}

	bool isSolveAble() {

		int blank_row_no = -1;
		vector<int> arr;
		for (int i = 0; i < boardSqSize; i++)
			for (int j = 0; j < boardSqSize; j++) {
				if (A[i][j])
					arr.push_back(A[i][j]);
				else
					blank_row_no = i;
			}
		int invCount = getInvCount(arr);
		bool boardSizeOdd = static_cast<bool>(boardSqSize & 1);
		if (boardSizeOdd && !(invCount & 1))
			return true;
		else if (!boardSizeOdd && ((blank_row_no + getInvCount(arr)) & 1))
			return true;
		return false;
	}


	static int getInvCount(const vector<int> &arr) {
		int inv_count = 0;
		for (int i = 0; i < arr.size() - 1; i++)
			for (int j = i + 1; j < arr.size(); j++)
				if (arr[i] > arr[j])
					inv_count++;

		return inv_count;
	}


	Node getNode(int direction, int zX = -1, int zY = -1) {
		if (A == nullptr || direction > 3)
			return *this;

		if (zX == -1 || zY == -1) {
			if (!getZeroPos(*this, zX, zY))
				return Node();
		}

		int zXnew = zX + dirX[direction];
		int zYnew = zY + dirY[direction];

		if (zXnew < 0 || zYnew < 0 || zXnew >= Node::boardSqSize || zYnew >= Node::boardSqSize)
			return Node();

		Node v = *this;
		swap(v.A[zX][zY], v.A[zXnew][zYnew]);
		return v;
	}

	static bool getZeroPos(const Node &node, int &zX, int &zY) {
		zX = zY = -1;
		for (int i = 0; i < Node::boardSqSize; i++) {
			for (int j = 0; j < Node::boardSqSize; j++)
				if (!node.A[i][j]) {
					zX = i, zY = j;
					return true;
				}
		}
		return false;
	}

	static int oppositeDirection(int direction) {
		switch (direction) {
			case LEFT:
				return RIGHT;
			case RIGHT:
				return LEFT;
			case UP:
				return DOWN;
			case DOWN:
				return UP;
			default:
				return EOF;
		}
	}

	bool isEmptyNode() const {
		return emptyNode;
	}
};
int Node::boardSqSize = 0;

ostream &operator<<(ostream &os, const Node &node) {
	if (!node.A) return os;
	for (int i = 0; i < Node::boardSqSize; i++) {
		for (int j = 0; j < Node::boardSqSize; j++)
			if (node.A[i][j])
				os << setw(PRINT_W) << (static_cast<int>(node.A[i][j])) << " ";
			else
				os << setw(PRINT_W) << "  " << " ";
		os << endl;
	}
	os << " ----------- " << std::endl;
	return os;
}
#endif //NPUZZLE_NODE_H



struct NodeInfo {
	bool isClosed;
	cost_t cost;
	parent_t parent;

	bool operator==(const NodeInfo &rhs) const {
		return parent == rhs.parent &&
		       cost == rhs.cost;
	}

	bool operator!=(const NodeInfo &rhs) const {
		return !(rhs == *this);
	}
};


class aStarSearch {
public:
	map<Node, NodeInfo> visited;//

	size_t openedCount;
	int max_depth;
	int nPushed;

	int heuristicType = 0;

	bool isValid(int x, int y) { return x >= 0 && y >= 0 && x < Node::boardSqSize && y < Node::boardSqSize; }

	static double HammingDistance(const Node &a, const Node &b) {
		int conflicts = 0;
		for (int i = 0; i < Node::boardSqSize; i++)
			for (int j = 0; j < Node::boardSqSize; j++)
				if (a.A[i][j] && a.A[i][j] != b.A[i][j])conflicts++;
		return conflicts;
	}

	static double ManHattan(const Node &a, const Node &b) {
		int sum = 0;
		puzzle_t pR[(Node::boardSqSize * Node::boardSqSize) + 1];
		puzzle_t pC[(Node::boardSqSize * Node::boardSqSize) + 1];
		for (int r = 0; r < Node::boardSqSize; r++) {
			for (int c = 0; c < Node::boardSqSize; c++) {
				pR[a.A[r][c]] = static_cast<puzzle_t>(r);
				pC[a.A[r][c]] = static_cast<puzzle_t>(c);
			}
		}
		for (int r = 0; r < Node::boardSqSize; r++)
			for (int c = 0; c < Node::boardSqSize; c++)
				if (b.A[r][c])
					sum += abs(pR[b.A[r][c]] - r) + abs(pC[b.A[r][c]] - c);
		return sum;
	}

	static double nLinearConflicts(const Node &a, const Node &b) {
		int conflicts = 0;
		puzzle_t pR[(Node::boardSqSize * Node::boardSqSize) + 1];
		puzzle_t pC[(Node::boardSqSize * Node::boardSqSize) + 1];
		for (int r = 0; r < Node::boardSqSize; r++) {
			for (int c = 0; c < Node::boardSqSize; c++) {
				pR[a.A[r][c]] = static_cast<puzzle_t>(r);
				pC[a.A[r][c]] = static_cast<puzzle_t>(c);
			}
		}

		for (int r = 0; r < Node::boardSqSize; r++) {
			for (int cl = 0; cl < Node::boardSqSize; cl++) {
				for (int cr = cl + 1; cr < Node::boardSqSize; cr++) {
					if (b.A[r][cl] && b.A[r][cr] && r == pR[b.A[r][cl]] && pR[b.A[r][cl]] == pR[b.A[r][cr]] &&
					    pC[b.A[r][cl]] > pC[b.A[r][cr]]) {
						conflicts++;
					}
				}
			}
		}

		// column conflicts -
		for (int c = 0; c < Node::boardSqSize; c++) {
			for (int rU = 0; rU < Node::boardSqSize; rU++) {
				for (int rD = rU + 1; rD < Node::boardSqSize; rD++) {
					if (b.A[rU][c] && b.A[rD][c] && c == pC[b.A[rU][c]] && pC[b.A[rU][c]] == pC[b.A[rD][c]] &&
					    pR[b.A[rU][c]] > pR[b.A[rD][c]]) {
						conflicts++;
					}
				}
			}
		}

		return conflicts;
	}

	static double LinearConflicts(const Node &a, const Node &b) {
		return ManHattan(a, b) + 2 * nLinearConflicts(a, b);
	}

	double Heuristic(const Node &a, const Node &b) {
		if (heuristicType == HAMMING_DISTANCE) return HammingDistance(a, b);
		if (heuristicType == MANHATTAN_DISTANCE) return ManHattan(a, b);
		if (heuristicType == LINEAR_CONFLICT) return LinearConflicts(a, b);
		return 0;
	}

	int AStarSearch(const Node &Start, const Node &Goal) {
		int nExpanded = 0;
		max_depth = 0;
		nPushed = 0;

		priority_queue<pair<double, Node> > openList;
		openList.push({0, Start});
		visited[Start] = {false, 0, EOF};

		while (!openList.empty()) {
			Node u = openList.top().second;
			openList.pop();
			++nExpanded;
			NodeInfo &uInfo = visited[u];
			uInfo.isClosed = true;

			max_depth = max(max_depth, visited[u].cost);

			if (u == Goal) {
				break;
			}

			if (uInfo.cost > LIMIT_DEPTH) {
				cout << "Height limit Exceeded @" << endl << u;
				break;
			}


			if (visited.size() > NODE_LIMIT) {
				cout << "Node limit Exceeded @" << endl << u;
				break;
			}

			int zX = -1, zY = -1;
			Node::getZeroPos(u, zX, zY);

			for (direction_t dir = 0; dir < 4; dir++) {
				int zXnew = zX + dirX[dir];
				int zYnew = zY + dirY[dir];
				if (isValid(zXnew, zYnew)) {
					Node v = u;
					swap(v.A[zX][zY], v.A[zXnew][zYnew]);

					bool isVisited = visited.find(v) != visited.end();
					if (isVisited && visited[v].isClosed)continue;

					double newCost = uInfo.cost + 1;
					if (!isVisited || newCost < visited[v].cost) {
						++nPushed;
						visited[v] = {false, static_cast<cost_t>(newCost), Node::oppositeDirection(dir)};
						double Priority = newCost + Heuristic(v, Goal);
						openList.push({-Priority, v});
					}
				}
			}

		}
		openedCount = visited.size();
		return nExpanded;
	}

	void setHeuristic(int heuristic = MANHATTAN_DISTANCE) {
		heuristicType = heuristic;
	}

	virtual ~aStarSearch() {
		heuristicType = 0;
		visited.clear();
	}
};








void printSolution(aStarSearch &starSearch, const Node &Start, const Node &Goal) {
	auto now = Goal;


	vector<Node> Path;
	while (starSearch.visited[now].parent_ != EOF) {
		Path.push_back(now);
		now = now.getNode(starSearch.visited[now].parent_);
	}
	Path.push_back(Start);
	reverse(Path.begin(), Path.end());
	for (auto &i : Path) cout << i;
}

void executeSearch(const Node &Start, const Node &Goal, int heuristic, bool printSol = true) {

	auto *starSearch = new aStarSearch();
	starSearch->setHeuristic(heuristic);
	int nExpanded = starSearch->AStarSearch(Start, Goal);

	cout << "No of Steps: " << (int) starSearch->visited[Goal].cost_ << endl;
	cout << "No of Nodes Expanded: " << nExpanded << endl;
	//cout << "No of Nodes Opened: " << starSearch->openedCount << endl;
	//cout << "No of Nodes Pushed: " << starSearch->nPushed << endl;
	cout << endl;
	fflush(stdout);

	if (printSol) printSolution(*starSearch, Start, Goal);
	delete starSearch;
}

int main() {
	freopen("in.txt", "r", stdin);
	int boardSqSize = 3;
	cin >> boardSqSize;
	Node::boardSqSize = boardSqSize;

	Node Goal;
	for (int i = 0; i < boardSqSize; i++)
		for (int j = 0; j < boardSqSize; j++)
			Goal.A[i][j] = static_cast<puzzle_t>(i * Node::boardSqSize + j + 1);
	Goal.A[Node::boardSqSize - 1][Node::boardSqSize - 1] = 0;

	Node Start;
	int x;
	for (int i = 0; i < boardSqSize; i++)
		for (int j = 0; j < boardSqSize; j++)
			cin >> x, Start.A[i][j] = static_cast<puzzle_t>(x);

	cout << "Start: \n" << Start;
	cout << "Goal: \n" << Goal;;

	if (!Start.isSolveAble()) {
		cout << "No Solution" << endl;
	} else {
		{
			cout << "# Linear Conflicts Heuristics: " << endl;
			executeSearch(Start, Goal, LINEAR_CONFLICT, false);

			cout << "# ManHattan Distance Heuristics: " << endl;
			executeSearch(Start, Goal, MANHATTAN_DISTANCE, false);

			cout << "#Hamming Distance Heuristics: " << endl;
			executeSearch(Start, Goal, HAMMING_DISTANCE, true);
		}
	}
}
