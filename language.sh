#!/bin/bash
#set -x
#
# Output
# ;skinparameter              <- Prefix => SKINPARAMETER
# ;379                        <- No of items
# ActivityArrowColor          <- Item 1 => SKINPARAMETER_ACTIVITYARRORCOLOR = "ActivityArrowColor";
# ActivityArrowFontColor
# ActivityArrowFontName
# ActivityArrowFontSize

echo "#ifndef LANGUAGE_H"
echo "#define LANGUAGE_H"
echo
echo "// Generated with $0 (see java plantuml.jar -language)"
echo
echo "#include <QString>"
echo "#include <QRegExp>"

prefix=""
java -jar ${1:-plantuml.jar} -language | while read line; do
    if [[ "${line}" =~ \;[a-z]+ ]]; then

	if [ -n "${all}" ]; then
		echo "const QRegExp ${prefix}_KEYWORDS = QRegExp(\"\\\\b(${all})\\\\b\");"
	fi

	prefix=$(echo ${line^^} | tr -d ';')
	all=""
	echo
	echo "// $(echo ${line} | tr -d ';') keywords"
	continue
    elif [[ "${line}" =~ \;[0-9]+ ]]; then
    	continue
    elif [ -z "${line}" -o "${line}" = ";EOF" ]; then
    	continue
    else
	suffix=$(echo ${line^^} | tr -d '@!'| tr -s ' ' '_')
        echo "const QString ${prefix}_${suffix} = \"${line}\";"

	if [ -z "${all}" ]; then
		all="${line}"
	else
		all="${all}|${line}"
	fi
    fi
done

echo
echo "#endif // LANGUAGE_H"
