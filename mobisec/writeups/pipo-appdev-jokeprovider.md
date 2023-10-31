# Solution

## Description of the problem

The *jokeprovider* challenge requires to develop an app that requests a content provider to obtain jokes of an author called *reyammer* and concatenate them.

## Solution

I used the `contentResolver.query()` method to query the table. The method requires four arguments:
- uri: The URI of our table, here "content://$PROVIDER\_NAME/jokes"
- projection: a list of requested columnts, here `id`, `author`, and `joke`
- selection: a filter on the rows similar to an *SQL* `WHERE` statement, here "author=?"
- selectionArgs: selection may use placeholders which concrete values are then supplied by this paramter, here {"reyammer"}
- order by: see *SQL* `ORDER BY`, here "id"

The method returns a cursor that we can use to iterate through the returned objects. We build a string based on the results in the `joke` column.
