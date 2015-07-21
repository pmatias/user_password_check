Check user passwords
====================

This Drupal module adds a new command to 
[Drush](https://github.com/drush-ops/drush) to check for weak credentials and 
compare against a password list file (or several).

## Installation

### Requirements:
* Drush 6.x (Drush 7.x support is coming)

### Manual Installation
* Clone this repository inside your `sites/all/modules` directory
```
  $ git clone git@github.com:pmatias/user_password_check.git
```
* Enable the module in the modules section (`admin/modules`).
  
### Via Drush (recommended)
Download and enable the module:
```
$ drush dl user_password_check
$ drush en -y user_password_check
```

## Usage


```
drush user-password-check 5,user3 --uid=2,3 --name=someguy,somegal --mail=billgates@microsoft.com
```
Check the users with name, id, or email 5 or user3, uids 2 and 3, 
names someguy and somegal, and email address of billgates@microsoft.com

It will print a warning if those users contains the user/password combination
user/user. e.g. user3/user3, someguy/someguy, and so on.


Alias for this command is upchk
```
drush upchk admin
```

```
drush upchk admin --block-user
```

It will block the user admin if the password is also admin.

```
drush upchk admin --block-user --password_list=file1.txt,file2.txt,...
```

It will block the user admin if the password is also admin or its password
is in one of the provided files.

## Known issues

* The verification doesn't play nice with big password lists against a big
users table (A.K.A. takes too much time)
 
## Roadmap

* Better performance
* Drupal backend 
