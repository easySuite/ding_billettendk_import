<?php

/**
 * @file
 * Administration side functionality.
 */

/**
 * Billetten.dk settings form.
 */
function ding_billettendk_import_settings_form($form, &$form_state) {
  $form = array();

  $form_state['cache'] = FALSE;

  $form['ding_billettendk_import_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
  );

  // Service URL field.
  $form['ding_billettendk_import_general']['ding_billettendk_import_service_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Service URL'),
    '#description' => t('URL to event provider service.'),
    '#default_value' => variable_get('ding_billettendk_import_service_url', 'https://v2.billetten.dk/feed/client/id/165/key/bbe49822cbd22a0cc1865183302a7347/format/xml/'),
    '#required' => TRUE,
  );

  $form['ding_billettendk_import_general']['ding_billettendk_import_nodes_status'] = array(
    '#type' => 'radios',
    '#title' => t('Nodes status'),
    '#description' => t('Imported nodes publication status. By default nodes will be published.'),
    '#options' => array(
      '1' => t('Published'),
      '0' => t('Unpublished')
    ),
    '#default_value' => variable_get('ding_billettendk_import_nodes_status', 1),
  );

  $processings = array(
    'locations' => array(
      'name' => 'Locations',
      'raw' => 'locations',
      'field' => 'library',
    ),
    'categories' => array(
      'name' => 'Categories',
      'raw' => 'categories',
      'field' => 'category',
    ),
  );

  foreach ($processings as $processing) {
    $form["ding_billettendk_import_{$processing['raw']}"] = array(
      '#type' => 'fieldset',
      '#title' => t("{$processing['name']} mapping"),
      '#collapsed' => FALSE,
      '#collapsible' => TRUE,
      '#prefix' => "<div id='{$processing['raw']}_wrapper'>",
      '#suffix' => '</div>',
    );

    $list = ding_billettendk_import_list($processing['raw']);

    $form["ding_billettendk_import_{$processing['raw']}"]["list"] = array(
      '#type' => 'markup',
      '#markup' => render($list),
    );

    $form["ding_billettendk_import_{$processing['raw']}"]["new"] = array(
      '#type' => 'fieldset',
      '#title' => t('Add new mapping'),
    );

    $form["ding_billettendk_import_{$processing['raw']}"]["new"]["feed_{$processing['field']}"] = array(
      '#type' => 'textfield',
      '#title' => t('Search string'),
      '#required' => TRUE,
    );

    $options_source = array();
    switch ($processing['raw']) {
      case 'locations':
        $options_source = ding_billettendk_import_get_libraries();
        break;

      case 'categories':
        $options_source = ding_billettendk_import_get_categories();
        break;
    }

    $form["ding_billettendk_import_{$processing['raw']}"]["new"]["ding_{$processing['field']}"] = array(
      '#type' => 'select',
      '#title' => t($processing['name']),
      '#options' => $options_source,
      '#required' => TRUE,
    );

    $form["ding_billettendk_import_{$processing['raw']}"]["new"]["{$processing['field']}_mapping_submit"] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#name' => "{$processing['field']}_mapping_submit",
      '#limit_validation_errors' => array(),
      '#submit' => array('ding_billettendk_import_stub'),
      '#ajax' => array(
        'callback' => "ding_billettendk_import_mappings_ajax_submit",
        'wrapper' => "{$processing['raw']}_wrapper",
        'method' => 'replace',
        'effect' => 'fade',
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#submit' => array('ding_billettendk_import_settings_form_submit'),
    '#limit_validation_errors' => array(),
  );

  return $form;
}

/**
 * Settings form submit.
 * @param $form
 * @param $form_state
 */
function ding_billettendk_import_settings_form_submit($form, &$form_state) {
  variable_set('ding_billettendk_import_service_url', $form['ding_billettendk_import_general']['ding_billettendk_import_service_url']['#value']);
  variable_set('ding_billettendk_import_nodes_status', $form['ding_billettendk_import_general']['ding_billettendk_import_nodes_status']['#value']);
}

/**
 * Stub function for dealing with form submit validation.
 */
function ding_billettendk_import_stub() {}

/**
 * AJAX submit handler.
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function ding_billettendk_import_mappings_ajax_submit($form, &$form_state) {
  $request_parent = $form_state['clicked_button']['#name'];

  if ($request_parent == 'library_mapping_submit') {
    $request_param = array('raw' => 'locations', 'field' => 'library');
  }
  else {
    $request_param = array('raw' => 'categories', 'field' => 'category');
  }

  $form_state['rebuild'] = TRUE;
  $mappings = variable_get("ding_billettendk_import_{$request_param['raw']}", array());

  $search_string = $form_state['input']["ding_{$request_param['field']}"];
  $mapping = $form_state['input']["feed_{$request_param['field']}"];

  $mappings[] = array(
    "feed_{$request_param['field']}" => $mapping,
    $request_param['field'] => $search_string,
  );

  if (!form_get_errors() && !empty($search_string) && !empty($mapping)) {
    $form_id = $form['#form_id'];

    // Get the unpopulated form.
    $form = form_get_cache($form_state['input']['form_build_id'], $form_state);

    // Rebuild it, without values
    $form_state['input'] = array();
    $form = form_builder($form_id, $form, $form_state);

    variable_set("ding_billettendk_import_{$request_param['raw']}", $mappings);
  }
  else {
    form_set_error('Mapping', 'Please re-check fields.');
  }

  $form["ding_billettendk_import_{$request_param['raw']}"]["list"] = ding_billettendk_import_list($request_param['raw']);

  return $form["ding_billettendk_import_{$request_param['raw']}"];
}

/**
 * Table builder.
 *
 * @param $type
 *
 * @return array
 */
function ding_billettendk_import_list($type) {
  $header = array(
    t('Search string'),
    t('Mapping'),
    t('Actions'),
  );

  $mappings = variable_get("ding_billettendk_import_{$type}", array());

  $rows = array();
  $i = 0;

  foreach ($mappings as $mapping) {
    $link = array(
      '#type' => 'link',
      '#title' => t('Delete'),
      '#href' => "billettendk/{$type}/delete/{$i}/nojs",
      '#attributes' => array(
        'class' => array('use-ajax'),
      ),
      '#ajax' => array(
        'wrapper' => "{$type}_table",
        'callback' => 'ding_billettendk_import_delete',
      ),
    );

    if ($type == 'locations') {
      $search_string = $mapping['feed_library'];
      $title = db_select('node', 'n')
        ->fields('n', array('title'))
        ->condition('nid', $mapping['library'])
        ->execute()
        ->fetchField();
    }
    else {
      $search_string = $mapping['feed_category'];
      $term = taxonomy_term_load($mapping['category']);
      $title = $term->name;
    }

    $rows[] = array(
      $search_string,
      $title,
      render($link),
    );
    $i++;
  }

  $table = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No mappings available.'),
    '#attributes' => array(
      'id' => "{$type}_table",
    ),
  );

  return $table;
}

/**
 * Fetching active libraries.
 *
 * @return array
 */
function ding_billettendk_import_get_libraries() {
  $libraries = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('type', 'ding_library')
    ->condition('status', 1)
    ->execute()
    ->fetchAllKeyed();

  return $libraries;
}

/**
 * Fetching event categories.
 *
 * @return array
 */
function ding_billettendk_import_get_categories() {
  $vocabulary_name = 'event_category';
  $vocabulary = taxonomy_vocabulary_machine_name_load($vocabulary_name);
  $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));

  $options = array();
  foreach ($terms as $term) {
    $options[$term->tid] = $term->name;
  }

  return $options;
}

/**
 * Delete mapping callback.
 *
 * @param $processing
 * @param $key
 * @param string $type
 */
function ding_billettendk_import_delete($processing, $key, $type = 'ajax') {
  if ($type == 'ajax') {
    drupal_add_library('system', 'drupal.ajax');

    $mappings = variable_get("ding_billettendk_import_{$processing}", array());
    unset($mappings[$key]);
    $mappings = array_values(array_filter($mappings));
    variable_set("ding_billettendk_import_{$processing}", $mappings);

    $list = ding_billettendk_import_list($processing);

    $commands = array();
    $commands[] = ajax_command_replace("#{$processing}_table", render($list));
    print ajax_render($commands);
  }
}

/**
 * Implements hook_ajax_render_alter().
 */
function ding_billettendk_import_ajax_render_alter(&$commands) {
  foreach ($commands as $k => $command) {
    if ($command['command'] == 'add_css') {
      unset($commands[$k]);
    }

    if ($command['command'] == 'insert' && $command['selector'] == 'head') {
      unset($commands[$k]);
    }
  }
}
