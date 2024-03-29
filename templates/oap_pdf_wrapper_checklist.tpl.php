<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
  <meta charset="UTF-8"/>
  <link href="form.css" type="text/css" rel="Stylesheet"/>
  <title><?php print $formtitle; ?></title>
</head>

<body class="checklist">
<div id="logo">&nbsp;</div>
<div id="header">
  <div class="formtitle"><?php print $formtitle; ?></div>
  <div id="userinfo">
    <p class="userdata">NAME: <?php print $name; ?></p>
    <p class="userdata">ID: <?php print $rmaxid; ?></p>
  </div>
  <table class="legendtable">
    <thead>
      <tr>
        <th class="sectionheader" colspan="2">Self-Assessment Rating Scale:</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td class="italictext">No Experience</td>
        <td>Theory or observation only during the past 12 months</td>
      </tr>
      <tr>
        <td class="italictext">Limited Experience</td>
        <td>Performed less than 12 times within the past 12 months and may need a review</td>
      </tr>
      <tr>
        <td class="italictext">Experience</td>
        <td>Performed at least once per month within the past 12 months and may need minimal assistance</td>
      </tr>
      <tr>
        <td class="italictext">Highly Skilled</td>
        <td>Performed on at least a weekly basis over the past 12 months; proficient</td>
      </tr>
    </tbody>
  </table>
</div>
<div id="content">
<?php print $body; ?>
</div>
</body>
</html>