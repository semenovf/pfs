#include "{{ output_file_header }}"

namespace {

{% for f in functions %}
  {% with f.arguments as arguments %}
    {{ f.returnType|default:"void" }} {{ name }}::{{ f.name }}({% include "arguments_types_names.txt" %});
  {% endwith %}
  {

  }
{% endfor %}

} // namespace::
