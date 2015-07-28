INTRODUCTION
------------

This Drupal module adds a new command to 
Drush (https://github.com/drush-ops/drush) to check for weak credentials and 
compare against a password list file (or several).


REQUIREMENTS
-------------
* Drush 6.x (Drush 7.x support is coming)


INSTALLATION
------------
* Clone this repository inside your `sites/all/modules` directory

$ git clone --branch 7.x-1.x http://git.drupal.org/sandbox/pable/2534886.git 
      user_password_check

* Enable the module in the modules section (`admin/modules`).


USAGE
-----

  drush user-password-check user

or

  drush upchk 5,user3 --uid=2,3 --name=someguy,somegal 
                                              --mail=billgates@microsoft.com

Check the users with name, id, or email 5 or user3, uids 2 and 3, 
names someguy and somegal, and email address of billgates@microsoft.com

It will print a warning if those users contains the user/password combination
user/user. e.g. user3/user3, someguy/someguy, and so on.


Alias for this command is upchk:

  drush upchk admin


To block the user admin if the password is also admin:
drush upchk admin --block-user


To block the user admin if the password is also admin or its password
is in one of the provided files:

  drush upchk admin --block-user --password-files=file1.txt,file2.txt,...



KNOWN ISSUES
------------

* The verification doesn't play nice with big password lists against a big
users table (A.K.A. takes too much time)
 

ROADMAP
-------

* Better performance
* Drupal backend 


MAINTAINERS
-----------
Pablo Fabregat - https://www.drupal.org/u/pablitt
