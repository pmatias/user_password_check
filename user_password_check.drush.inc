<?php
/**
 * @file
 * New user-password-check command for drush.
 */

 /**
  * Implements hook_drush_help().
  */
function user_password_check_drush_help($section) {
  switch ($section) {
    case 'drush:user-password-check':
      return 'Check user(s) credentials basic security.';
  }
}

/**
 * Implements hook_drush_command().
 */
function user_password_check_drush_command() {
  $items = array();

  $items['user-password-check'] = array(
    'description' => 'Check user(s) credentials basic security.',
    'aliases' => array('upchk'),
    'arguments' => array(
      'users' => 'A comma delimited list of uids, user names, or email addresses of users to check.',
    ),
    'examples' => array(
      'drush user-password-check 5,user3 --uid=2,3 --name=someguy,somegal --mail=billgates@microsoft.com' =>
      dt('Check the users with name, id, or email 5 or user3, uids 2 and 3, names someguy and somegal, and email address of billgates@microsoft.com'),
    ),
    'options' => array(
      'password-files' => array(
        'description' => 'A comma separated list of passwords file(s).',
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
 * Check user password.
 */
function drush_user_password_check($users = '') {
  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');

  $block_user = drush_get_option('block-user', FALSE);

  $password_files = drush_get_option('password-files');
  if ($password_files) {
    $password_list = array();
    foreach (explode(",", $password_files) as $password_file) {
      $password_list = array_merge($password_list, file($password_file));
    }
    asort($password_list);
  }

  $user_list = _drush_user_get_users_from_options_and_arguments($users);

  if (empty($user_list)) {
    // No uids were specified, let's get them all.
    // Not using drush_db_select since it does not have support for JOIN.
    $user_list = db_query("SELECT u.uid, u.name, u.pass 
      FROM {users} u 
      LEFT JOIN {authmap} a ON u.uid = a.uid
      WHERE u.uid > 0
      AND u.status = 1
      AND a.uid IS NULL")
      ->fetchAll();
  }

  foreach ($user_list as $user) {
    $user_object = ($user->name) ? $user : user_load($user);

    // We want to know, first, if the user has the
    // user/user user/password combination.
    if (user_check_password($user_object->name, $user_object)) {
      if ($block_user) {
        user_block_user_action($user_object);
        drush_log(dt('User !user has been blocked.', array('!user' => $user_object->name)), 'success');
      }
      else {
        drush_log(dt('User !user has weak credentials.', array('!user' => $user_object->name)), 'warning');
      }
      continue;
    }

    // We also want to know if the user has a password that belongs
    // to the provided password list.
    if ($password_list) {
      foreach ($password_list as $password) {
        if (user_check_password(trim($password), $user_object)) {
          if ($block_user) {
            user_block_user_action($user_object);
            drush_log(dt('User !user has been blocked.', array('!user' => $user_object->name)), 'success');
          }
          else {
            drush_log(dt('User !user has weak credentials.', array('!user' => $user_object->name)), 'warning');
          }
          break;
        }
      }
    }
  }

}
