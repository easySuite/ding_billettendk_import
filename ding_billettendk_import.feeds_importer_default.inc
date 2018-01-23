<?php
/**
 * @file
 * ding_billettendk_import.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function ding_billettendk_import_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'billetten_dk_events_import';
  $feeds_importer->config = array(
    'name' => 'Billetten.dk events import',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => 0,
        'use_pubsubhubbub' => 0,
        'designated_hub' => '',
        'request_timeout' => '',
        'auto_scheme' => 'http',
        'accept_invalid_cert' => 0,
        'cache_http_result' => 0,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExXml',
      'config' => array(
        'context' => array(
          'value' => '//EVENT',
        ),
        'sources' => array(
          'event_no' => array(
            'name' => 'EVENT_NO',
            'value' => 'EVENT_NO',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '1',
          ),
          'event_name' => array(
            'name' => 'EVENT_NAME',
            'value' => 'EVENT_NAME',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '2',
          ),
          'event_notes' => array(
            'name' => 'EVENT_NOTES',
            'value' => 'EVENT_NOTES',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '3',
          ),
          'category_type' => array(
            'name' => 'CATEGORY_TYPE',
            'value' => 'CATEGORY_TYPE',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '4',
          ),
          'subcategory_type' => array(
            'name' => 'SUBCATEGORY_TYPE',
            'value' => 'SUBCATEGORY_TYPE',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '5',
          ),
          'event_image' => array(
            'name' => 'EVENT_IMAGE',
            'value' => 'EVENT_IMAGES/IMAGE[@key="standard"]',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '6',
          ),
          'location_name' => array(
            'name' => 'LOCATION_NAME',
            'value' => 'SHOWS/SHOW/LOCATION_NAME',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '7',
          ),
          'location_street' => array(
            'name' => 'LOCATION_STREET',
            'value' => 'SHOWS/SHOW/LOCATION_STREET',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '8',
          ),
          'location_zip' => array(
            'name' => 'LOCATION_ZIP',
            'value' => 'SHOWS/SHOW/LOCATION_ZIP',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '9',
          ),
          'location_city' => array(
            'name' => 'LOCATION_CITY',
            'value' => 'SHOWS/SHOW/LOCATION_CITY',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '10',
          ),
          'price' => array(
            'name' => 'PRICE',
            'value' => 'SHOWS/SHOW/PRICES/PRICEGROUP/@STD_PRICE',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '11',
          ),
          'show_date' => array(
            'name' => 'SHOW_DATE',
            'value' => 'SHOWS/SHOW/SHOW_TIME',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '12',
          ),
          'show_link' => array(
            'name' => 'SHOW_LINK',
            'value' => 'SHOWS/SHOW/SHOW_LINK',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '13',
          ),
          'availability' => array(
            'name' => 'AVAILABILITY',
            'value' => 'SHOWS/SHOW/AVAILABILITY',
            'raw' => 0,
            'inner' => 0,
            'debug' => 0,
            'weight' => '14',
          ),
        ),
        'display_errors' => 0,
        'debug_mode' => 0,
        'source_encoding' => array(
          0 => 'auto',
        ),
        'use_tidy' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'authorize' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'event_no',
            'target' => 'guid',
            'unique' => 1,
            'language' => 'und',
          ),
          1 => array(
            'source' => 'event_name',
            'target' => 'title',
            'unique' => FALSE,
            'language' => 'und',
          ),
          2 => array(
            'source' => 'category_type',
            'target' => 'field_ding_event_category',
            'term_search' => '0',
            'autocreate' => 1,
            'language' => 'und',
          ),
          3 => array(
            'source' => 'event_image',
            'target' => 'field_ding_event_list_image:uri',
            'unique' => FALSE,
            'language' => 'und',
          ),
          4 => array(
            'source' => 'event_image',
            'target' => 'field_ding_event_title_image:uri',
            'unique' => FALSE,
            'language' => 'und',
          ),
          5 => array(
            'source' => 'event_notes',
            'target' => 'field_ding_event_body',
            'unique' => FALSE,
            'language' => 'und',
          ),
          6 => array(
            'source' => 'price',
            'target' => 'field_ding_event_price',
            'unique' => FALSE,
            'language' => 'und',
          ),
          7 => array(
            'source' => 'subcategory_type',
            'target' => 'field_ding_event_tags',
            'term_search' => '0',
            'autocreate' => 1,
            'language' => 'und',
          ),
          8 => array(
            'source' => 'show_date',
            'target' => 'field_ding_event_date:start',
            'timezone' => '__SITE__',
            'language' => 'und',
          ),
          9 => array(
            'source' => 'show_date',
            'target' => 'field_ding_event_date:end',
            'timezone' => '__SITE__',
          ),
        ),
        'insert_new' => '1',
        'update_existing' => '2',
        'update_non_existent' => 'skip',
        'input_format' => 'ding_wysiwyg',
        'skip_hash_check' => 1,
        'bundle' => 'ding_event',
        'language' => 'und',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '0',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['billetten_dk_events_import'] = $feeds_importer;

  return $export;
}
