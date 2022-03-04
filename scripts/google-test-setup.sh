

grepme="last changed rev"
if [ -e $1 ]
then
	cd $1
	version_local=$(svn info | grep -i "$grepme" | cut -d":" -f2)
	version_remote=$(svn info -r HEAD | grep -i "$grepme" | cut -d":" -f2)
	if [ "$version_local" = "$version_remote" ]
	then
		echo "googltest: up to date"
		updated="no"
	else
		echo "googletest: updating ... "
		svn update
		updated="yes"
	fi
else
	mkdir -p $1
	#svn checkout http://googletest.googlecode.com/svn/trunk/ $1
	git clone git@github.com:google/googletest.git $1
	updated="yes"
	cd $1
fi


DIR_BUILD=$2
should_build=$updated
if [ -e $DIR_BUILD ]
then
	echo "build exists"
	should_build='no'
else
    should_build='yes'
fi

if [ "$should_build" = "yes" ]
then
	rm -rf $DIR_BUILD
	mkdir $DIR_BUILD
	cd $DIR_BUILD
	cmake -DBUILD_SHARED_LIBS=ON -Dgtest_build_samples=ON -G"Unix Makefiles" ..
	make
fi

