#pragma once

namespace {

class {{ name }}
{% if baseClasses %}
    : {% for b in baseClasses %}b.inheritanceMode b.baseType{% if not forloop.last %}, {% endif %}{% endfor %}
{% endif %}
{
private:
{% for m in members %}
  {{ m.type }} {{ m.name }};
{% endfor %}

public:
{% for f in functions %}
  {% with f.arguments as arguments %}
    {{ f.returnType|default:"void" }} {{ f.name }}({% include "arguments_types_names.txt" %});
  {% endwith %}
{% endfor %}
}; // class {{ name }}

} // namespace 
