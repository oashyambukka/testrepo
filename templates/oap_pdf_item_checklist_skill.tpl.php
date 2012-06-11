<tr>
  <td><?php $element['#title']; ?></td>
  <td><?php print ($element['#value'] == 1) ? '&#9632;' : '&#9633;'; ?>&nbsp;No Experience</td>
  <td><?php print ($element['#value'] == 2) ? '&#9632;' : '&#9633;'; ?>&nbsp;Limited Experience</td>
  <td><?php print ($element['#value'] == 3) ? '&#9632;' : '&#9633;'; ?>&nbsp;Experience</td>
  <td><?php print ($element['#value'] == 4) ? '&#9632;' : '&#9633;'; ?>&nbsp;Highly Skilled</td>
</tr>
