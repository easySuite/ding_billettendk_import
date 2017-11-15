# Ding Billetten.dk events import

Module created for import of events from billetten.dk service.

#### Configuration
On **@admin/config/ding/ding_billettendk_import** we have configuration form where we have to set URL to the feed with events, default status of node (published/unpublished), map event locations to the local libraries and map event categories to the local categories.

#### Mapping
In case of locations: if term in search string field match with location name from the field, event node will be saved with relation to mapped library, for rest of events the Location fields will be filled.

In case of categories: if term in search string field match with category from the field, event node will be saved with relation to mapped category, for rest of events new category will be created automatically.

In dependency of "Availability" feed item value will be rendered text on "Book button". So in this way if:
* availability = 1 => text = "Solf out"
* availability = 2 => text = "Few available tickets"
* availability = 3 => text = "Book a ticket"
