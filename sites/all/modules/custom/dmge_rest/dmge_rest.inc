<?php
// dmge_rest.inc
/**
 * Callback for creating note resources.
 *
 * @param object $data
 * @return object
 */
function _dmge_rest_create($data) {
  global $user;
  if (isset($data) &&  is_array($data)) $data=(object) $data;
  unset($data->id);
  $data->uid = $user->uid;
  $data->created = time();
  $data->modified = time();

  if (!isset($data->subject)) {
    return services_error('Missing note attribute subject', 406);
  }

  if (!isset($data->note)) {
    return services_error('Missing note attribute note', 406);
  }

  dmge_rest_write_note($data);

  return (object)array(
    'id' => $data->id,
    'uri' => services_resource_uri(array('note', $data->id)),
  );
}

 // dmge_rest.inc
/**
 * Callback for updating note resources.
 *
 * @param int $id
 * @param object $data
 * @return object
 */
function _dmge_rest_update($id, $data) {
  global $user;
  $note = dmge_rest_get_note($id);

  unset($data->created);
  $data->id = $id;
  $data->uid = $note->uid;
  $data->modified = time();

  dmge_rest_write_note($data);

  return (object)array(
    'id' => $id,
    'uri' => services_resource_uri(array('note', $id)),
  );
}
/**
 * Callback for retrieving note resources.
 *
 * @param int $id
 * @return object
 */
function _dmge_rest_retrieve($id) {
  return dmge_rest_get_note($id);
}

/**
 * Callback for deleting note resources.
 *
 * @param int $id
 * @return object
 */
function _dmge_rest_delete($id) {
  dmge_rest_delete_note($id);

  return (object)array(
    'id' => $id,
  );
}

function _dmge_rest_index($page, $parameters) {
  global $user;

  $notes = array();
  $res = db_query('SELECT * FROM {note} WHERE uid=:uid ORDER BY modified DESC',
           array(':uid' => $user->uid));

  foreach ($res as $note) {
    $notes[] = $note;
  }

  return $notes;
}

/**
 * Access callback for the note resource.
 *
 * @param string $op
 *  The operation that's going to be performed.
 * @param array $args
 *  The arguments that will be passed to the callback.
 * @return bool
 *  Whether access is given or not.
 */

function _dmge_rest_access($op, $args) {
  global $user;
  $access = FALSE;

  switch ($op) {
    case 'view':
      $note = dmge_rest_get_note($args[0]);
      $access = user_access('note resource view any note');
      $access = $access || $note->uid == $user->uid && user_access('note resource view own notes');
      break;
    case 'update':
      $note = dmge_rest_get_note($args[0]->id);
      $access = user_access('note resource edit any note');
      $access = $access || $note->uid == $user->uid && user_access('note resource edit own notes');
      break;
    case 'delete':
      $note = dmge_rest_get_note($args[0]);
      $access = user_access('note resource delete any note');
      $access = $access || $note->uid == $user->uid && user_access('note resource delete own notes');
      break;
  }
  $access = TRUE;

  return $access;
}
?>
