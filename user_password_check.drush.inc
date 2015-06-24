<?php

use Drush\User\UserList;
use Drush\User\UserListException;

/**
 * Implementation of hook_drush_command().
 */
function user_password_check_drush_command() {
  $items = array();
  
  $items['user-password-check'] = array(
    'description' => 'Check user(s) basic credentials security.',
    'aliases' => array('upchk'),
    'arguments' => array(
      'users' => 'A comma delimited list of uids, user names, or email addresses of users to check.',
    ),
    'examples' => array(
      'drush user-password-check 5,user3 --uid=2,3 --name=someguy,somegal --mail=billgates@microsoft.com' =>
        'Check the users with name, id, or email 5 or user3, uids 2 and 3, names someguy and somegal, and email address of billgates@microsoft.com',
    ),
    'options' => array(
      'password-list' => array(
        'description' => 'A comma separated file list of passwords.',
        'required' => FALSE,
        'example-value' => 'password_list_1.txt,password_list_2.txt,...',
      ),
      'block-user' => array(
        'description' => 'Block the user if it has weak credentials.',
        'required' => FALSE,
      ),
    ),
  );  
  
  return $items;
}


/**
* Check user password
*/
function drush_user_password_check($inputs = '') {
  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
  
  $block_user = drush_get_option('block-user', FALSE);
  
  if (drush_get_option('password-list')) {
    $password_list = array();
    foreach (explode(",", drush_get_option('password-list')) as $pass_file) {
      $password_list = array_merge($password_list, file($pass_file));
    }
  }

  try {
    // remember the authmap table!
    $userlist_collection = new UserList($inputs);
    $userlist = $userlist_collection->accounts;
  }
  catch (UserListException $e) {
    // No user option or argument was passed, so we get them all.
    $userlist = db_query('SELECT u.uid, u.name, u.pass 
      FROM users u 
      LEFT JOIN authmap a ON u.uid = a.uid
      WHERE u.uid > 0
      AND u.status = 1
      AND a.uid IS NULL')
    ->fetchAll();
  } 
  
  foreach ($userlist as $user) {
    $user = ($user->name) ? $user : $user->account;
    
    // We want to know first if the user has the
    // user/user user/password combination.
    if (user_check_password($user->name, $user)) {
      if ($block_user) {
        user_block_user_action($user);
        drush_log(dt('User !user has been blocked.', array('!user' => $user->name)), 'success');        
      }
      else {
        drush_log(dt('User !user has weak credentials.', array('!user' => $user->name)), 'warning');
      }
      continue;
    }
    
    // We also want to know if the user has a password that belongs 
    // to the password list.
    if ($password_list) {
      foreach($password_list as $password){
        if(user_check_password(trim($password), $user)) {
          if ($block_user) {
            user_block_user_action($user);
            drush_log(dt('User !user has been blocked.', array('!user' => $user->name)), 'success');
          }
          else {
            drush_log(dt('User !user has weak credentials.', array('!user' => $user->name)), 'warning');
          }
          break;
        }
      }
    }
  }
  
}
