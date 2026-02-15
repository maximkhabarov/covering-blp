#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

//static inline double cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }

int strtoi(string s) {
	int x = atoi(s.c_str());
	return x;
}
string inttostr(int number)
{
	stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}

vector<int> parse_string(string& s) {
	vector<int> res;
	int k1 = 0;
	int k2 = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ' ') {
			k2 = i;
			string tmp = s.substr(k1, k2 - k1);
			int r = strtoi(tmp);
			res.push_back(r);
			k1 = k2;
		}
	}
	if (k2 != s.size() - 1) {
		string tmp = s.substr(k1, s.size() - k1);
		int r = strtoi(tmp);
		res.push_back(r);
	}
	return res;
}

void load_info(string filename, int k, vector<vector<int>> &wcnf, vector<int> & weights) {
	wcnf.clear();
	weights.clear();
	ifstream in;
	in.open(filename);
	string s;
	while (in.good()) {
		getline(in, s);
		if ((s.size() > 0) && (s[0] != 'c') && (s[0] != 'p')) {
			vector<int> r = parse_string(s);
			vector<int> f;
			for (int i = 1; i < r.size()-1; i++) {
				f.push_back(r[i]);
			}
			if (r[0] != k){
				weights.push_back(r[0]);
				wcnf.push_back(f);
			}
		}
	}
	in.close();
}

int greedy_iteration(int f, int k, vector<int> &solution, vector<vector<int>> wcnf, vector<int> weights) {
	vector<vector<int>> watches(k, vector<int>(0));
	vector<int> satisfied(wcnf.size(),0);
	for (int i = 0; i < wcnf.size(); i++) {
		for (int j = 0; j < wcnf[i].size(); j++) {
			watches[wcnf[i][j]].push_back(i);
		}
		satisfied[i] = wcnf[i].size();
	}
	solution = vector<int>(k + 1, 0);

	int u = 0;
	while (u < f) {

		vector<int> penalties(k + 1,0);
		//evaluate for each agent the number of falsified x their weight
		for (int i = 1; i < k; i++) {
			if (solution[i] == 0) {
				for (int j = 0; j < watches[i].size(); j++) {
					if (satisfied[watches[i][j]] == 1) {

						penalties[i] += weights[watches[i][j]];
						//58 93487
					}
				}
			}
		}
		int ind_sol = 0;
		int min_penalty = 300000;
		for (int i = 1; i < k ; i++) {
			if (solution[i] == 0) {
				if (penalties[i] < min_penalty) {
					ind_sol = i;
					min_penalty = penalties[i];
				}
			}
		}
		cout << "Adding " << ind_sol << " to solution\n";
		solution[ind_sol] = ++u;
		for (int i = 0; i < watches[ind_sol].size(); i++) {
			satisfied[watches[ind_sol][i]]--;
		}
	}
	int function_value = 0;
	for (int i = 0; i < satisfied.size(); i++) {
		if (satisfied[i] <= 0) {
			function_value += weights[i];
		}
	}
	for (int i = 1; i < k; i++) {
		if (solution[i] == 0) {
			cout << -i << " ";
		}
		else {
			cout << i << " ";
		}
	}
	cout << endl;
	cout << "Function value = " << function_value << endl;


	//recompute function value
	int final_value = 0;
	for (int i = 0; i < wcnf.size(); i++) {
		int sf = wcnf[i].size();
		for (int j = 0; j < wcnf[i].size(); j++) {
			if (solution[wcnf[i][j]] > 0) {
				sf--;
			}
			if (sf == 0) {
				final_value += weights[i];
			}
		}
	}
	cout << "final value " << final_value << endl;


	return final_value;
}


int greedy_iteration2(int f, int k, vector<int>& solution, vector<vector<int>> wcnf, vector<int> weights) {
	vector<vector<int>> watches(k, vector<int>(0));
	vector<int> satisfied(wcnf.size(), 0);
	for (int i = 0; i < wcnf.size(); i++) {
		for (int j = 0; j < wcnf[i].size(); j++) {
			watches[wcnf[i][j]].push_back(i);
		}
		satisfied[i] = wcnf[i].size();
	}
	solution = vector<int>(k + 1, 0);

	int u = 0;
	while (u < f) {

		vector<int> penalties(k + 1, 0);
		//evaluate for each agent the number of falsified x their weight
		for (int i = 1; i < k; i++) {
			if (solution[i] == 0) {
				for (int j = 0; j < watches[i].size(); j++) {
					if (satisfied[watches[i][j]] == 1) {
						penalties[i] += weights[watches[i][j]];
						//58 93487
					}
				}
			}
		}
		int ind_sol = 0;
		int min_penalty = 300000;
		for (int i = 1; i < k; i++) {
			if (solution[i] == 0) {
				if (penalties[i] < min_penalty) {
					ind_sol = i;
					min_penalty = penalties[i];
				}
			}
		}
		cout << "Adding " << ind_sol << " to solution\n";
		solution[ind_sol] = ++u;
		for (int i = 0; i < watches[ind_sol].size(); i++) {
			satisfied[watches[ind_sol][i]]--;
		}
	}
	int function_value = 0;
	for (int i = 0; i < satisfied.size(); i++) {
		if (satisfied[i] <= 0) {
			function_value += weights[i];
		}
	}
	for (int i = 1; i < k; i++) {
		if (solution[i] == 0) {
			cout << -i << " ";
		}
		else {
			cout << i << " ";
		}
	}
	cout << endl;
	cout << "Function value = " << function_value << endl;
	vector<int> solution_greedy(solution);
	cout << "Attempting local search" << endl;

	vector<int> solution_record(solution_greedy);
	int f_record = function_value;

	for (int p = 1; p < k; p++) {
		if (solution[p] > 0) {
			int current_f = function_value;
			solution[p] = 0;
			for (int i = 0; i < watches[p].size(); i++) {
				if (satisfied[watches[p][i]] == 0) {
					current_f -= weights[watches[p][i]];
				}
				satisfied[watches[p][i]]++;
			}
			/*int curf = 0;

			for (int i = 0; i < satisfied.size(); i++) {
				if (satisfied[i] <= 0) {
					curf += weights[i];
				}
			}
		//	cout << "CURF " << curf << " C_F " << current_f << endl;
			if (curf != current_f) {
				cout << " Something is wrong!\n";
				getchar();
			}*/

			//check here just in case
			vector<int> penalties(k + 1, 0);
			//evaluate for each agent the number of falsified x their weight
			for (int i = 1; i < k; i++) {
				if (solution[i] == 0) {
					for (int j = 0; j < watches[i].size(); j++) {
						if (satisfied[watches[i][j]] == 1) {

							penalties[i] += weights[watches[i][j]];
							//58 93487
						}
					}
				}
			}
			int ind_sol = 0;
			int min_penalty = 300000;
			for (int i = 1; i < k; i++) {
				if (solution[i] == 0) {
					if (penalties[i] < min_penalty) {
						ind_sol = i;
						min_penalty = penalties[i];
					}
				}
			}
			if (current_f + min_penalty < f_record) {
				cout << "New record OMG!\n";
				cout<<"replacing "<< p << " by " << ind_sol << endl;
				f_record = current_f + min_penalty;
				solution_record[p] = 0;
				cout << "Function value " << f_record << endl;
				solution_record[ind_sol] = k;

				for (int i = 1; i < k; i++) {
					if (solution[i] == 0) {
						cout << -i << " ";
					}
					else {
						cout << i << " ";
					}
				}
			}
			solution[p] = 1;
			for (int i = 0; i < watches[p].size(); i++) {
				satisfied[watches[p][i]]--;
			}
		}
	}

	//recompute function value
	int final_value = 0;
	for (int i = 0; i < wcnf.size(); i++) {
		int sf = wcnf[i].size();
		for (int j = 0; j < wcnf[i].size(); j++) {
			if (solution_record[wcnf[i][j]] > 0) {
				sf--;
			}
			if (sf == 0) {
				final_value += weights[i];
			}
		}
	}
	cout << "final value " << final_value << endl;

	return 1;

}

int main(int argc, char* argv[]) {
	const size_t test_number = std::stoi(argv[1]);
    const size_t stations_number = std::stoi(argv[2]);
    const size_t clients_number = std::stoi(argv[3]);
    const std::string radius = argv[4];
    const size_t satisfied_clients_lower_bound = std::stoi(argv[5]);
    const size_t seed = std::stoi(argv[6]);
    const size_t x = std::stoi(argv[7]);
    const size_t y = std::stoi(argv[8]);
	assert(argc == 9);

	std::string folder = "../cnf-generator/WCNFs/";
	std::string subfolder_name =
		"test_"
		+ std::to_string(test_number)
		+ "_s"
		+ std::to_string(stations_number)
		+ "_c"
		+ std::to_string(clients_number)
		+ "_radius"
		+ radius
		+ "_SClientsLBound"
		+ std::to_string(satisfied_clients_lower_bound)
		+ "_seed"
		+ std::to_string(seed)
		+ "_x"
		+ std::to_string(x)
		+ "_y"
		+ std::to_string(y) + "/";
	const std::string path = folder + subfolder_name + "1.wcnf";

	vector<vector<int>> wcnf;
	vector<int> weights;

    load_info(path, clients_number + 1, wcnf, weights);

    {
        vector<int> sol;
        assert(greedy_iteration(0, stations_number + 1, sol, wcnf, weights) == 0);
    }

    int prev_ok_f = 0;
    for (int f = 0; f <= stations_number; f++)
    {
    	vector<int> sol;
        if (clients_number - greedy_iteration(f, stations_number + 1, sol, wcnf, weights) >= satisfied_clients_lower_bound)
        {
            prev_ok_f = f;
        }
    }

	int prev_ok_f_2 = 0;
    for (int f = 0; f <= stations_number; f++)
    {
    	vector<int> sol;
        if (clients_number - greedy_iteration2(f, stations_number + 1, sol, wcnf, weights) >= satisfied_clients_lower_bound)
        {
            prev_ok_f_2 = f;
        }
    }

    cout << "Greedy result is " << stations_number - prev_ok_f << endl;
    cout << "Greedy2 results is " << stations_number - prev_ok_f_2 << endl;
    cout << "Final results is " << max(stations_number - prev_ok_f_2, stations_number - prev_ok_f) << endl;
}