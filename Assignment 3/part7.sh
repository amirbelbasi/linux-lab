#!/bin/bash

read -p "1, 2 or 3: " inp
case $inp in
	1) echo -e "1\n22\n333\n4444\n55555";;

	2) echo -e "          *\n\n        *   *\n\n      *   *   *\n\n    *   *   *   *\n\n  *   *   *   *   *\n\n*   *   *   *   *   *\n\n*   *   *   *   *   *\n\n  *   *   *   *   *\n\n    *   *   *   *\n\n      *   *   *\n\n        *   *\n\n          *";;

	3) echo -e "|__\n|  |__\n|  |  |__\n|  |  |  |__\n|  |  |  |  |__";;
esac