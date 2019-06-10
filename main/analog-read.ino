int read_analog_input() {
  // round to
  int round_to = 5;

  // input bounds
  int input_upper_bound = 1020;
  int input_lower_bound = 0;
  // output bounds
  int output_upper_bound = 40;
  int output_lower_bound = 1;

  // ranges
  int input_range = input_upper_bound - input_lower_bound;
  int output_range = output_upper_bound - output_lower_bound;

  // read analog input
  int pin_read = analogRead(analog_pin);

  // input % or range
  int input_val = pin_read - input_lower_bound;
  float input_percent = (float)input_val / (float)input_range;

  // convert into output ranges
  int output_val = (float)output_lower_bound + (input_percent * (float)output_range);
  return output_val;
}
