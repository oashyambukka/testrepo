        <tr>
          <td class="fixedwidth"><?php print $element['#title']; ?></td>
          <td><?php print ($element['#value'] == 1) ? '<span style="font-size: 20px;">&bull;</span>' : '&nbsp;&nbsp;'; ?>&nbsp;No Experience</td>
          <td><?php print ($element['#value'] == 2) ? '<span style="font-size: 20px;">&bull;</span>' : '&nbsp;&nbsp;'; ?>&nbsp;Limited Experience</td>
          <td><?php print ($element['#value'] == 3) ? '<span style="font-size: 20px;">&bull;</span>' : '&nbsp;&nbsp;'; ?>&nbsp;Experienced</td>
          <td><?php print ($element['#value'] == 4) ? '<span style="font-size: 20px;">&bull;</span>' : '&nbsp;&nbsp;'; ?>&nbsp;Highly Skilled</td>
        </tr>
