/*
 * 작성자: 윤정도
 * 생성일: 10/10/2023 7:51:46 AM
 * =====================
 *
 */

// ReSharper disable all

#include <algorithm>
#include <set>
#include <vector>
#include <iostream>
#include <functional>
#include <limits>
#include <string>
#include <cassert>
#include <sstream>

#include <Windows.h>

using namespace std;

struct skill_info
{
	string name;				// 스킬명
	int duplicated_count;		// 중첩횟수
	int index;
};

struct auto_function
{
	auto_function(function<void()>&& fn) : call(move(fn)) {}
	~auto_function() { call(); }

	function<void()> call;
	
};

#define BR													printf("\n")
#define BR_SEPERATOR										printf("======================================\n")

#define RED													"\x1b[040;31m" 
#define GRAY												"\x1b[040;39m"
#define GREEN												"\x1b[040;92m" 

#define ERROR_INVALID_SELECTED_MENU							10000
#define ERROR_INVALID_SELECTED_MENU_MSG						"메뉴를 똑바로 선택해주세요.\n"

#define ERROR_EMPTY_SKILL_CORE_LIST							10001
#define ERROR_EMPTY_SKILL_CORE_LIST_MSG						"스킬 코어 목록을 입력해주세요.\n"

#define ERROR_INVALID_DUPLICATED_SKILL_COUNT				10002
#define ERROR_INVALID_DUPLICATED_SKILL_COUNT_MSG			"스킬 코어 목록의 중첩 횟수의 총합이 3의 배수가 아닙니다.\n"

#define ERROR_INVALID_SKILL_CORE_INPUT_1					10010
#define ERROR_INVALID_SKILL_CORE_INPUT_1_MSG				"스킬 코어 정보가 잘못되었습니다. ex)스킬명:중첩횟수\n"

#define ERROR_INVALID_SKILL_CORE_INPUT_2					10011
#define ERROR_INVALID_SKILL_CORE_INPUT_2_MSG				"스킬 코어 정보가 잘못되었습니다.(중첩횟수가 이상함.) ex)스킬명:중첩횟수\n"

#define ERROR_INVALID_SKILL_CORE_INPUT_3					10012
#define ERROR_INVALID_SKILL_CORE_INPUT_3_MSG				"해당 스킬 코어 정보가 이미 존재합니다.\n"

#define ERROR_CANNOT_CALCULATE_COMBINATION_1				10013
#define ERROR_CANNOT_CALCULATE_COMBINATION_1_MSG			"조합 계산에 실패했습니다. (모든 스킬코어의 중첩횟수 총합이 0이거나 3의배수가 아닙니다.).\n"

#define ERROR_INVALID_SKILL_CORE_NUMBER						10020
#define ERROR_INVALID_SKILL_CORE_NUMBER_MSG					"올바르지 않은 스킬 코어 번호입니다.\n"

#define ERROR_INVALID_SKILL_CORE_LIST_INPUT					10021
#define ERROR_INVALID_SKILL_CORE_LIST_INPUT_MSG				"올바르지 않은 스킬 코어 목록 정보입니다. ex)스킬1:중첩횟수,스킬2:중첩횟수 와 같이 콤마단위로 입력할 것\n"

#define ERROR_INVALID_INPUT									10022
#define ERROR_INVALID_INPUT_MSG								"올바르지 입력입니다.\n"

#define ERROR_MSG_OF(def_error)								def_error##_MSG

vector<skill_info> core;						// <스킬명, 중첩횟수>
vector<pair<string, function<void()>>> menus;	// 메뉴
int selected_menu;								// 선택된 메뉴 번호
int last_error_code;							// 오류 코드

void print_menu();
void select_menu();
void print_error_msg();

// 메뉴들
void add_skill_core();
void add_skill_core_list();
void remove_skill_core();
void clear_skill_core_list();
void calculate_combination();
void program_exit() { exit(0); }

int main() {

	for (;;) {
		last_error_code = 0;

		print_menu();
		select_menu();
		print_error_msg();
		BR;
	}

	return 0;
}

template <typename T>
bool read_input(const char* msg, T& value) {
	// https://stackoverflow.com/questions/10349857/how-to-handle-wrong-data-type-input
	if (std::cout << msg && !(std::cin >> value)) {
		std::cin.clear();					//clear bad input flag
		std::cin.ignore(LLONG_MAX, '\n');	//discard input
		return false;
	}
	return true;
}

void print_menu() {
	if (core.size() > 0) {
		int i{};
		for (auto skill : core) {
			printf("[%d] %s:%d\n", ++i, skill.name.c_str(), skill.duplicated_count);
		}
		BR;
	}

	menus = {
		{ "dummy",								function<void()>([]() {})	},
		{ "스킬 추가하기\n",						add_skill_core				},
		{ "스킬 목록 추가하기\n",					add_skill_core_list			},
		{ "스킬 제거하기\n",						remove_skill_core			},
		{ "스킬 목록 비우기\n",					clear_skill_core_list		},
		{ "조합 계산하기\n",						calculate_combination		},
		{ "프로그램 종료\n",						program_exit				}
	};

	int menu_counter = 0;
	auto it = menus.empty() ? menus.end() : ++menus.begin();
	for (; it != menus.end(); ++it) {
		printf("%d. %s", ++menu_counter, it->first.c_str());
	}
	printf("메뉴 선택> ");
}



void select_menu() {
	selected_menu = 0;

	if (!read_input("", selected_menu)) {
		last_error_code = ERROR_INVALID_INPUT;
		return;
	}

	if (selected_menu < 1 || selected_menu > menus.size()) {
		last_error_code = ERROR_INVALID_SELECTED_MENU;
		return;
	}

	menus[selected_menu].second();
}



void print_error_msg() {
	switch (last_error_code) {
	case ERROR_INVALID_SELECTED_MENU:				printf(RED ERROR_MSG_OF(ERROR_INVALID_SELECTED_MENU) GRAY);				break;
	case ERROR_EMPTY_SKILL_CORE_LIST:				printf(RED ERROR_MSG_OF(ERROR_EMPTY_SKILL_CORE_LIST) GRAY);				break;
	case ERROR_INVALID_DUPLICATED_SKILL_COUNT:		printf(RED ERROR_MSG_OF(ERROR_INVALID_DUPLICATED_SKILL_COUNT) GRAY);	break;
	case ERROR_INVALID_SKILL_CORE_INPUT_1:			printf(RED ERROR_MSG_OF(ERROR_INVALID_SKILL_CORE_INPUT_1) GRAY);		break;
	case ERROR_INVALID_SKILL_CORE_INPUT_2:			printf(RED ERROR_MSG_OF(ERROR_INVALID_SKILL_CORE_INPUT_2) GRAY);		break;
	case ERROR_INVALID_SKILL_CORE_INPUT_3:			printf(RED ERROR_MSG_OF(ERROR_INVALID_SKILL_CORE_INPUT_3) GRAY);		break;
	case ERROR_CANNOT_CALCULATE_COMBINATION_1:		printf(RED ERROR_MSG_OF(ERROR_CANNOT_CALCULATE_COMBINATION_1) GRAY);	break;
	case ERROR_INVALID_SKILL_CORE_NUMBER:			printf(RED ERROR_MSG_OF(ERROR_INVALID_SKILL_CORE_NUMBER) GRAY);			break;
	}
}


void add_skill_core_impl(const string& input) {
	string::size_type i = input.find(':');

	if (i == string::npos || i == 0 || i == input.length() - 1) {
		last_error_code = ERROR_INVALID_SKILL_CORE_INPUT_1;
		return;
	}

	string skill_name = input.substr(0, i);
	int duplicated_count = atoi(input.substr(i + 1, input.length() - i + 1).c_str());

	if (duplicated_count == 0) {
		last_error_code = ERROR_INVALID_SKILL_CORE_INPUT_2;
		return;
	}

	for (int i = 0; i < core.size(); ++i) {
		if (core[i].name == skill_name) {
			last_error_code = ERROR_INVALID_SKILL_CORE_INPUT_3;
			return;
		}
	}

	core.push_back({ skill_name, duplicated_count, int(core.size()) });
}

void add_skill_core() {
	string input;

	if (!read_input("스킬 정보를 입력해주세요> ", input)) {
		last_error_code = ERROR_INVALID_INPUT;
		return;
	}

	add_skill_core_impl(input);
	
}

void add_skill_core_list() {
	printf("스킬 목록 정보를 입력해주세요> ");
	string input;
	cin.ignore();
	std::getline(std::cin, input);

	auto fn_split = [](string s, char delim) {
		vector<string> splited;
		stringstream ss(s);
		string temp;

		while (getline(ss, temp, delim)) {
			splited.push_back(temp);
		}

		return splited;
	};

	vector<string> skill_info_strs = fn_split(input, ',');

	if (skill_info_strs.size() == 0) {
		last_error_code = ERROR_INVALID_SKILL_CORE_LIST_INPUT;
		return;
	}

	for (int i = 0; i < skill_info_strs.size(); ++i) {
		add_skill_core_impl(skill_info_strs[i]);
	}
}

void remove_skill_core() {
	int core_number;
	if (!read_input("제거할 스킬코어 번호 입력>", core_number)) {
		last_error_code = ERROR_INVALID_INPUT;
		return;
	}

	int core_index = core_number - 1;
	if (core_index < 0 || core_index >= core.size()) {
		last_error_code = ERROR_INVALID_SKILL_CORE_NUMBER;
		return;
	}

	core.erase(core.begin() + core_index);
}

void clear_skill_core_list() {
	core.clear();
}

void calculate_combination() {

	int total_duplicated_skill_count = 0;	// 중첩된 스킬 수
	int output_core_count = 0;				// ex) 중첩포함 스킬 수가 9개면 3개의 코어가 필요함

	// 마스터리 코어: 스킬 3개로 구성됨
	// 당연히 스킬 정보 3개는 모두 달라야함.
	struct mastery_core
	{
		skill_info* skill[3];

		mastery_core() {
			skill[0] = nullptr;
			skill[1] = nullptr;
			skill[2] = nullptr;
		}

		mastery_core(const mastery_core& other) {
			skill[0] = other.skill[0];
			skill[1] = other.skill[1];
			skill[2] = other.skill[2];
		}

		void print(bool accent_main_skill = false) {
			for (int i = 0; i < 3; ++i) {
				if (i == 0 && accent_main_skill)	// 메인 스킬
					printf(GREEN "[%d] %s\n" GRAY, i + 1, skill[i]->name.c_str());
				else
					printf("[%d] %s\n", i + 1, skill[i]->name.c_str());
			}
		}

		// true:	1스킬이 동일하고 2, 3 스킬이 순서 상관없이 동일한지 검사
		// false:	1, 2, 3스킬이 순서 상관없이 동일 한지 검사
		// false 예시
		// 1) 폭류권
		// 2) 소혼 장막
		// 3) 귀참

		// 1) 소혼 장막
		// 2) 귀참
		// 3) 폭류권
		// 이 2개의 마스터리 코어를 동일한 것으로 취급함

		bool equal(bool main, mastery_core* other) {
			if (main) {
				if (skill[0] == other->skill[0]) {
					if (skill[1] == other->skill[2] &&
						skill[2] == other->skill[1])
						return true;

					if (skill[1] == other->skill[1] &&
						skill[2] == other->skill[2])
						return true;
				}
			} else {
				
				bool exist[3]{};

				for (int i = 0; i < 3; ++i) {
					for (int j = 0; j < 3; j++) {
						if (skill[i] == other->skill[j]) {
							exist[i] = true;
							break;
						}
					}
				}

				if (all_of(exist, exist + 3, [](bool v) { return v == true;})) {
					return true;
				}

			}

			return false;
		}
	};

	// 마스터리 코어 조합
	struct combination
	{
		vector<mastery_core*> cores;

		combination(int core_count) : cores(core_count, nullptr) {}

		void reset() { std::fill(cores.begin(), cores.end(), nullptr); }

		void print(bool accent_main_skill = false) {
			if (cores.size() == 0) return;
			assert(cores[cores.size() - 1] != nullptr);

			for (int i = 0; i < cores.size(); ++i) {
				cores[i]->print(accent_main_skill);
				BR;
			}

			BR_SEPERATOR;
		}


		// 동일한 조합인지 검사
		// 컴비네이션을 구성하는 마스터리 코어들이 순서 상관없이 동일한 종류인 경우
		bool equal(combination* other) {
			int count = cores.size();
			assert(other->cores.size() == count);	//  컴비네이션을 구성하는 코어 수는 당연히 동일해야한다.

			vector<bool> exist(count, false);

			for (int i = 0; i < count; ++i) {
				for (int j = 0; j < count; ++j) {
					if (cores[i]->equal(false, other->cores[j])) {
						exist[i] = true;
						break;
					}
				}
			}

			return all_of(exist.begin(), exist.end(), [](bool v) { return v == true; });
		}
	};
	
	vector<mastery_core> all_mastery_cores;			// 구성가능한 모든 마스터리 코어정보
	vector<bool> visit(core.size(), false);
	mastery_core tmp_mastery_core;

	for (auto& skill : core) {
		total_duplicated_skill_count += skill.duplicated_count;
	}

	if (total_duplicated_skill_count == 0 || total_duplicated_skill_count % 3 != 0) {
		last_error_code = ERROR_CANNOT_CALCULATE_COMBINATION_1;
		return;
	}

	output_core_count = total_duplicated_skill_count / 3;


	// 모든 스킬중 3개를 뽑았을 때의 마스터리 코어 정보를 얻는다.
	function<void(int, int)> fn_collect_mastery_cores;
	fn_collect_mastery_cores = [&](int count, int index) mutable {

		// 함수 진입시 수행
		visit[index] = true;
		tmp_mastery_core.skill[count - 1] = &core[index];

		// 함수로 종료시 수행
		auto_function auto_fn([&]() {
			tmp_mastery_core.skill[count - 1] = nullptr;
			visit[index] = false;
		});

		if (count == 3) {

			// 메인코어가 동일할 때 2, 3번째 슬롯이 같은 코어가 존재하는 경우 추가할 필요가 없다.
			for (int i = 0; i < all_mastery_cores.size(); ++i) {
				if (all_mastery_cores[i].equal(true, &tmp_mastery_core)) {
					return;
				}
			}


			all_mastery_cores.push_back(tmp_mastery_core);
			return;
		}

		for (int i = 0; i < core.size(); ++i) {
			if (visit[i]) continue;
			fn_collect_mastery_cores(count + 1, i);
		}

		tmp_mastery_core.skill[count - 1] = nullptr;
		visit[index] = false;
	};

	for (int i = 0; i < core.size(); ++i) {
		fn_collect_mastery_cores(1, i);
	}

	// TEST-PRINT
	for (int i = 0; i < all_mastery_cores.size(); ++i) {
	 	// all_mastery_cores[i].print();
	}

	visit = vector<bool>(all_mastery_cores.size(), false);

	combination tmp_combination(output_core_count);
	vector<int> tmp_duplicated_skill_counter(core.size(), 0);
	vector<combination> all_combinations;						// 순서 고려한 모든 조합
	vector<combination*> all_combinations_regardless_of_order;	// 순서 상관없는 조합

	function<void(int, int)> fn_collect_combinations;
	fn_collect_combinations = [&](int count, int index) {
		mastery_core* cur_core = &all_mastery_cores[index];

		// 함수 진입시 수행
		visit[index] = true;
		tmp_combination.cores[count - 1] = cur_core;

		for (int i = 0; i < 3; ++i) {
			tmp_duplicated_skill_counter[cur_core->skill[i]->index] += 1;
		}


		// 함수로 종료시 수행
		auto_function auto_fn([&]() {
			tmp_combination.cores[count - 1] = nullptr;
			visit[index] = false;

			for (int i = 0; i < 3; ++i) {
				tmp_duplicated_skill_counter[cur_core->skill[i]->index] -= 1;
			}
		});


		// 1. 코어들의 메인 스킬중 중복된 스킬이 없어야한다.
		for (int i = 0; i < count; ++i) {
			mastery_core* lhs = tmp_combination.cores[i];
			for (int j = i + 1; j < count; j++) {
				mastery_core* rhs = tmp_combination.cores[j];
				if (lhs->skill[0] == rhs->skill[0]) {
					return;
				}
			}
		}
	
		// 2. 스킬 중첩 횟수가 초과해버린 경우가 있으면 더이상 진행하지 않도록 CPU 낭비를 줄인다.
		for (int i = 0; i < core.size(); ++i) {
			if (tmp_duplicated_skill_counter[i] > core[i].duplicated_count) {
				return;
			}
		}

		if (count == output_core_count) {

			// 3. 코어 갯수만큼 모두 모인 경우 최종적으로 모든 스킬횟수가 동일한지 검사
			for (int i = 0; i < core.size(); ++i) {
				if (tmp_duplicated_skill_counter[i] != core[i].duplicated_count) {
					return;
				}
			}

			all_combinations.push_back(tmp_combination);
			return;
		}

		for (int i = index + 1; i < all_mastery_cores.size(); ++i) {
			if (visit[i]) continue;
			fn_collect_combinations(count + 1, i);
		}
	};

	for (int i = 0; i < all_mastery_cores.size(); ++i) {
		fn_collect_combinations(1, i);
	}

	/*for (int i = 0; i < all_combinations.size(); ++i) {
		all_combinations[i].print();
	}*/

	std::vector<bool> removed(all_combinations.size(), false);

	for (int i = 0; i < all_combinations.size(); ++i) {

		if (removed[i])
			continue;

		all_combinations_regardless_of_order.push_back(&all_combinations[i]);

		for (int j = i + 1; j < all_combinations.size(); ++j) {
			if (all_combinations[i].equal(&all_combinations[j])) {
				removed[j] = true;
			}
		}
	}

	for (int i = 0; i < all_combinations_regardless_of_order.size(); ++i) {
		all_combinations_regardless_of_order[i]->print(false);
	}
}